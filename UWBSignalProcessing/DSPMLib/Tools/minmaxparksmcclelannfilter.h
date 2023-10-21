#ifndef MINMAXPARKSMCCLELANNFILTER_H
#define MINMAXPARKSMCCLELANNFILTER_H
#include <math.h>
#include <vector>

class MinMaxParksMcClelannFilter {
public:
  enum FilterType { Band = 1, Diff = 2, Hilbert = 3 };
  MinMaxParksMcClelannFilter(const int &num_taps, const int &n_bands,
                             std::vector<double> &bands_edge,
                             const std::vector<double> &desire,
                             const std::vector<double> &weight,
                             const FilterType &type, const int &grid_density,
                             const int &max_iterations);
  ~MinMaxParksMcClelannFilter();

  std::vector<double> GetFilterCoeff();
  int GetRemezStatus();
  ///Метод расчета коэффициентов ИХ
  /// num_band - число полос
  /// tap_count - длина фильтра
  /// fir_coeff - вектор
private:
  static const int remez_errnomem = -1;
  static const int remez_warnmaxiter = 1;
  static const int remez_noerr = 0;

  /////Проверяем, отработал ли метод
  //      bool RemezFailed() const
  //      {
  //          return (status<0);
  //      }

  ///////////Секция внутренних членов класса
  bool remez_symmetry;
  int gridsize;
  int r; /// половина от коэффициентов фильтра
  std::vector<double> grid; /// густая сетка частот
  std::vector<double> d; ///АЧХ, "натянутое" на густую сетку
  std::vector<double> w; ///Весовые коэффициенты, "натянутые" на густую сетку
  std::vector<int>
      ext; ///Вектор экстремальных частот, расположенных на нашей сетке
  std::vector<double> ad; //значение b у ОПЕНГЕЙМА и ШАФЕРА [r+1]
  std::vector<double> x; //[r+1]
  std::vector<double> y; //значение c у ОПЕНГЕЙМА и ШАФЕРА [r+1]
  std::vector<double> e; /// Вектор ошибки
  std::vector<double> h; ///вектор отсчетов ИХ получившегося фильтра
  std::vector<int> foundExt;
  int status;

  ////////////////////////////////////////////

  ////////////////Основные методы класса

  ///Создание густой сетки частот и "натягивание" на нее желаемой АЧХ фильтра и
  ///весовых коэффициентов
  void CreateDenseGrid(const int &numtaps, const int &numband,
                       std::vector<double> &bands,
                       const std::vector<double> &des,
                       const std::vector<double> &weight,
                       const int &grid_density);

  ///Размещение экстремальных частот на нашей густой сетке
  void InitialGuess();

  ///Рассчет параметров

  void CalcParams();

  ///Считаем реальную ЧХ фильтра в данной частотной точке
  double ComputeA(const double &freq);

  ///Рассчет функции ошибки (отличие желаемой ЧХ от получающегося)

  void CalcError();

  ///Поиск минимума и максимума функции ошибки на нашей частотной сетке

  void Search();

  ///Нахождение ИХ (по формулам из таблицы  Рабинера-Гоулда)

  void FreqSample(const int &N, std::vector<double> A);

  ///Проверка на сходимость

  short isDone();

  ///Непосредственно сам алгорит Ремеза

  void remez(const int &num_taps, const int &n_bands,
             std::vector<double> &bands_edge, const std::vector<double> &desire,
             const std::vector<double> &weight, const FilterType &type,
             const int &grid_density, const int &max_iterations);
};

#endif // MINMAXPARKSMCCLELANNFILTER_H
