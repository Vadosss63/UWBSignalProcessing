#include "minmaxparksmcclelannfilter.h"
#include <math.h>

MinMaxParksMcClelannFilter::MinMaxParksMcClelannFilter(
    const int &num_taps, const int &n_bands, std::vector<double> &bands_edge,
    const std::vector<double> &desire, const std::vector<double> &weight,
    const FilterType &type, const int &grid_density, const int &max_iterations)
    : h(num_taps) {

  remez(num_taps, n_bands, bands_edge, desire, weight, type, grid_density,
        max_iterations);
}

MinMaxParksMcClelannFilter::~MinMaxParksMcClelannFilter() {}

std::vector<double> MinMaxParksMcClelannFilter::GetFilterCoeff() {

  if (GetRemezStatus() != 0)
    h.clear();
  return (std::move(h));
}

int MinMaxParksMcClelannFilter::GetRemezStatus() { return status; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MinMaxParksMcClelannFilter::CreateDenseGrid(
    const int &numtaps, const int &numband, std::vector<double> &bands,
    const std::vector<double> &des, const std::vector<double> &weight,
    const int &grid_density) {
  int j, k;

  double delf = 0.5 / (grid_density * r);

  ///Фильтры типа Гильберта и дифференциатора имеют нечетную симметрю, и по
  ///этому: grid[0] = max(delf, band[0])

  if ((remez_symmetry == 0) && (delf > bands[0]))
    bands.at(0) = delf;

  j = 0;

  for (int band_index = 0; band_index < numband; band_index++) {
    grid.at(j) = bands.at(2 * band_index);

    // названия переменных отсылают нас к оригинальному алгоритму

    double lowf = bands.at(2 * band_index);
    double highf = bands.at(2 * band_index + 1);
    double lowd = des.at(2 * band_index);
    double highd = des.at(2 * band_index + 1);

    k = static_cast<int>(((highf - lowf) / delf) +
                         0.5); // 0.5 для округления (зачем? ПРОВЕРИТЬ)!!!!
    //Костыльный костыль из-за кривого округления типа
    if (k == 0)
      k = 1;
    double deld = (highd - lowd) / k;
    for (int i = 0; i < k; i++) {
      d.at(j) = lowd;
      w.at(j) = weight.at(band_index);
      grid.at(j) = lowf;
      lowf += delf;
      lowd += deld;
      j++;
    }
    grid.at(j - 1) = highf;
  }

  ///Согласно тому, что говорилось выше про нечетную симметрию фильтров.
  ///Последней точкой в сетке не может быть частота Найквиста (0.5), но если у
  ///нас четная длина фильтра, то точку можно оставить

  if ((remez_symmetry == 0) && (grid.at(gridsize - 1) > (0.5 - delf)) &&
      (numtaps % 2)) {
    grid.at(gridsize - 1) = 0.5 - delf;
  }
}

//////////////////////////////////////////////////////////////////////
void MinMaxParksMcClelannFilter::InitialGuess() {
  for (int i = 0; i <= r; i++)
    ext.at(i) = i * (gridsize - 1) / r;
}

//////////////////////////////////////////////////////////////////////
///ЭКВИВАЛЕНТНАЯ ЗАДАЧА АПРОКСИМАЦИИ
/////////////////////////////////////////////////////////////////////
void MinMaxParksMcClelannFilter::CalcParams() {
  double sign, xi, delta, denom, numer;

  // Ищем x

  for (int i = 0; i <= r; i++)
    x.at(i) = cos(2 * M_PI * grid.at(ext.at(i)));

  //Вычисляем ad  - Oppenheim & Schafer eq 7.132

  int ld = (r - 1) / 15 + 1;
  for (int i = 0; i <= r; i++) {
    denom = 1.0;
    xi = x.at(i);
    for (int j = 0; j < ld; j++) {
      for (int k = j; k <= r; k += ld)
        if (k != i)
          denom *= 2.0 * (xi - x.at(k));
    }
    if (fabs(denom) < 0.00001)
      denom = 0.00001;
    ad.at(i) = 1.0 / denom;
  }

  //Рассчитываем дельту  - Oppenheim & Schafer eq 7.131 (Отклонение частотной
  //характеристики)

  numer = denom = 0;
  sign = 1;
  for (int i = 0; i <= r; i++) {
    numer += ad.at(i) * d.at(ext.at(i));
    denom += sign * ad.at(i) / w.at(ext.at(i));
    sign = -sign;
  }
  delta = numer / denom;
  sign = 1;

  //Вычисляем y  - Oppenheim & Schafer eq 7.133b (

  for (int i = 0; i <= r; i++) {
    y.at(i) = d.at(ext.at(i)) - sign * delta / w.at(ext.at(i));
    sign = -sign;
  }
}

////////////////////////////////////////////////////////////////////
double MinMaxParksMcClelannFilter::ComputeA(const double &freq) {
  double xc, c, denom, numer;

  denom = numer = 0;
  xc = cos(2 * M_PI * freq);
  for (int i = 0; i <= r; i++) {
    c = xc - x.at(i);
    if (fabs(c) < 1.0e-7) {
      numer = y.at(i);
      denom = 1;
      break;
    }
    c = ad.at(i) / c;
    denom += c;
    numer += c * y.at(i);
  }
  return numer / denom;
}

/////////////////////////////////////////////////////////////
void MinMaxParksMcClelannFilter::CalcError() {
  for (int i = 0; i < gridsize; i++) {
    double A = ComputeA(grid.at(i));
    e.at(i) = w.at(i) * (d.at(i) - A);
  }
}

///////////////////////////////////////////////////////////////////
void MinMaxParksMcClelannFilter::Search() {

  int i, j, k, l, extra; //Счетчики
  int up, alt;

  //Выделяем место под найденные экстремумы

  k = 0;

  //Проверяем, есть ли экстремум в 0

  if (((e.at(0) > 0.0) && (e.at(0) > e.at(1))) ||
      ((e.at(0) < 0.0) && (e.at(0) < e.at(1))))
    foundExt.at(k++) = 0;

  //Проверяем на экстремумы внутри густой сетки

  for (i = 1; i < gridsize - 1; i++) {
    if (((e.at(i) >= e.at(i - 1)) && (e.at(i) > e.at(i + 1)) &&
         (e.at(i) > 0.0)) ||
        ((e.at(i) <= e.at(i - 1)) && (e.at(i) < e.at(i + 1)) &&
         (e.at(i) < 0.0)))
      foundExt.at(k++) = i;
  }

  //Проверяем на экстремум на частоте Найквиста

  j = gridsize - 1;
  if (((e.at(j) > 0.0) && (e.at(j) > e.at(j - 1))) ||
      ((e.at(j) < 0.0) && (e.at(j) < e.at(j - 1))))
    foundExt.at(k++) = j;

  //Убираем "экстра" экстремумы

  extra = k - (r + 1);

  while (extra > 0) {
    if (e.at(foundExt.at(0)) > 0.0)
      up = 1; //первый максимум
    else
      up = 0; //первый минимум

    l = 0;
    alt = 1;
    for (j = 1; j < k; j++) {
      if (fabs(e.at(foundExt.at(j))) < fabs(e.at(foundExt.at(l))))
        l = j; //новая наименьшая ошибка
      if ((up) && (e.at(foundExt.at(j)) < 0.0))
        up = 0; //переход на минимум
      else if ((!up) && (e.at(foundExt.at(j)) > 0.0))
        up = 1; //переход на максимум
      else {
        alt = 0;
        break; //Нашли два непротивоположных экстремума
      } //Удаляем наименьший из них
    } //После завершения цикла все экстремумы будут противоположны

    //Если у нас один экстремум и все они противоположны, удаляем наименьший
    //первый/последний

    if ((alt) && (extra == 1)) {
      if (fabs(e.at(foundExt.at(k - 1))) < fabs(e.at(foundExt.at(0))))
        l = foundExt.at(k - 1); //Удаляем последний экстремум
      else
        l = foundExt.at(0); //Удаляем первый экстремум
    }

    for (j = l; j < k; j++) //Цикл для удаления
    {
      foundExt.at(j) = foundExt.at(j + 1);
    }
    k--;
    extra--;
  }

  for (i = 0; i <= r; i++) {
    ext.at(i) = foundExt.at(i); //Копируем найденные экстремумы в ext
  }
}

////////////////////////////////////////////////////////////////////////////////////
void MinMaxParksMcClelannFilter::FreqSample(const int &N,
                                            std::vector<double> A) {
  int n, k;
  double x, val, M;

  M = (N - 1.0) / 2.0;
  if (remez_symmetry == 1) {
    if (N % 2) {
      for (n = 0; n < N; n++) {
        val = A.at(0);
        x = 2 * M_PI * (n - M) / N;
        for (k = 1; k <= M; k++)
          val += 2.0 * A.at(k) * cos(x * k);
        h.at(n) = val / N;
      }
    } else {
      for (n = 0; n < N; n++) {
        val = A.at(0);
        x = 2 * M_PI * (n - M) / N;
        for (k = 1; k <= (N / 2 - 1); k++)
          val += 2.0 * A.at(k) * cos(x * k);
        h.at(n) = val / N;
      }
    }
  } else {
    if (N % 2) {
      for (n = 0; n < N; n++) {
        val = 0;
        x = 2 * M_PI * (n - M) / N;
        for (k = 1; k <= M; k++)
          val += 2.0 * A.at(k) * sin(x * k);
        h.at(n) = val / N;
      }
    } else {
      for (n = 0; n < N; n++) {
        val = A.at(N / 2) * sin(M_PI * (n - M));
        x = 2 * M_PI * (n - M) / N;
        for (k = 1; k <= (N / 2 - 1); k++)
          val += 2.0 * A.at(k) * sin(x * k);
        h.at(n) = val / N;
      }
    }
  }
}

/////////////////////////////////////////////////
short MinMaxParksMcClelannFilter::isDone() {
  int i;
  double min, max, current;

  min = max = fabs(e.at(ext.at(0)));
  for (i = 1; i <= r; i++) {
    current = fabs(e.at(ext.at(i)));
    if (current < min)
      min = current;
    if (current > max)
      max = current;
  }
  if (((max - min) / max) < 0.0001)
    return 1;
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void MinMaxParksMcClelannFilter::remez(
    const int &num_taps, const int &n_bands, std::vector<double> &bands_edge,
    const std::vector<double> &desire, const std::vector<double> &weight,
    const MinMaxParksMcClelannFilter::FilterType &type, const int &grid_density,
    const int &max_iterations) {
  // double *mem;
  int i, iter; //*imem;
  double c;
  std::vector<double> taps;

  status = remez_noerr;

  if (type == Band)
    remez_symmetry = true;
  else
    remez_symmetry = false;

  r = num_taps / 2; //число экстремумов
  if ((num_taps % 2) && (remez_symmetry))
    r++;

  /*
   * Predict dense grid size in advance for memory allocation
   *   .5 is so we round up, not truncate
   */
  gridsize = 0;
  for (i = 0; i < n_bands; i++) {
    gridsize +=
        static_cast<int>(2 * r * grid_density *
                             (bands_edge.at(2 * i + 1) - bands_edge.at(2 * i)) +
                         .5f); ////////////!!!!!! КРИВЫЕ КАСТЫ
  }
  if (!remez_symmetry) {
    gridsize--;
  }

  /*
   * Dynamically allocate memory for arrays with proper sizes
   */
  //       mem = new double (4 * (gridsize + r + 1) );
  //      imem = new int((3 * r + 1));

  //    Случай отказа в выделении памяти. На данном железе крайне сомнительно
  //        if (NULL == mem || NULL == imem)
  //        {
  //            status = REMEZ_ERRNOMEM;
  //            goto finish;
  //        }

  //В оригинальном алгоритме выделение памяти велось посредством нарезания
  //кусков сырого пула памяти
  foundExt.resize(2 * r + 1);
  ext.resize(r + 2);

  grid.resize(gridsize + 2);
  d.resize(gridsize + 2);
  w.resize(gridsize + 2);
  e.resize(gridsize + 2);
  taps.resize(r + 2);
  x.resize(r + 2);
  y.resize(r + 2);
  ad.resize(r + 2);

  //Создание густой сетки частот
  CreateDenseGrid(num_taps, n_bands, bands_edge, desire, weight, grid_density);
  InitialGuess();

  //Для диффиринцирующего фильтра
  if (type == Diff) {
    for (i = 0; i < gridsize; i++) {
      /* D[i] = D[i]*Grid[i]; */
      if (d[i] > 0.0001)
        w[i] = w[i] / grid[i];
    }
  }

  /*
   * For odd or Negative symmetry filters, alter the
   * D[] and W[] according to Parks McClellan
   */
  if (remez_symmetry) {
    if (num_taps % 2 == 0) {
      for (i = 0; i < gridsize; i++) {
        c = cos(M_PI * grid.at(i));
        d.at(i) /= c;
        w.at(i) *= c;
      }
    }
  } else {
    if (num_taps % 2) {
      for (i = 0; i < gridsize; i++) {
        c = sin(2 * M_PI * grid.at(i));
        d.at(i) /= c;
        w.at(i) *= c;
      }
    } else {
      for (i = 0; i < gridsize; i++) {
        c = sin(M_PI * grid.at(i));
        d.at(i) /= c;
        w.at(i) *= c;
      }
    }
  }

  /*
   * Perform the Remez Exchange algorithm
   */
  for (iter = 0; iter < max_iterations; iter++) {
    CalcParams();
    CalcError();
    Search();

    if (isDone())
      break;
  }
  if (iter == max_iterations) {
    /* printf("Reached maximum iteration count.\nResults may be bad.\n"); */
    status = remez_warnmaxiter;
  }

  CalcParams();

  /*
   * Find the 'taps' of the filter for use with Frequency
   * Sampling.  If odd or Negative symmetry, fix the taps
   * according to Parks McClellan
   */
  for (i = 0; i <= num_taps / 2; i++) {
    if (remez_symmetry) {
      if (num_taps % 2)
        c = 1;
      else
        c = cos(M_PI * static_cast<double>(i) / num_taps);
    } else {
      if (num_taps % 2)
        c = sin(2 * M_PI * static_cast<double>(i) / num_taps);
      else
        c = sin(M_PI * static_cast<double>(i) / num_taps);
    }
    taps.at(i) = ComputeA(static_cast<double>(i) / num_taps) * c;
  }

  /*
   * Frequency sampling design with calculated taps
   */
  FreqSample(num_taps, taps);
}
