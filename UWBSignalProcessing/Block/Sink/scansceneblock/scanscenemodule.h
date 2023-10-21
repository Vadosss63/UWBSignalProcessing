#ifndef SCANBUFFERSCENEMODULE_H
#define SCANBUFFERSCENEMODULE_H

#include "csignal.h"
#include "sectoritem.h"
#include "sinkmodule.h"
#include <QObject>
#include <algorithm>

class ScanBufferSceneModule : public QObject,
                              public SinkModule<csignal<double>> {
  Q_OBJECT
public:
  ScanBufferSceneModule(QObject *parent = nullptr);
  ~ScanBufferSceneModule() override = default;

  void Operate() override;
  // выполняет установку вертикального смещения и усиления
  void SetValue(int verticalShift, int gain, uint8_t coefStreatch);
  // устанавливает режим цветного отображения
  void SetColorView(bool isColorView) noexcept;
  // производит обновление изображения через определенное количество дорожек
  // дальности
  void SetUpdateScan(bool updateScan, int updateStep = 64) noexcept;
  // устанавливает режим отображения с белой линией
  void SetWhiteLine(bool isWhiteLine) noexcept;

  // возврощает указатель на матрицу отображения
  std::vector<SectorBaseItem *> &DataMatrix();

  /// Настройка цветного отображения
  void SetColor1(const QColor &color) noexcept;
  void SetColor2(const QColor &color) noexcept;
  void SetColor3(const QColor &color) noexcept;

signals:
  //Изменение элемента матрицы
  void ScanUpdated();

private:
  void UpdateSector(uint32_t azimuth);
  void ShowWhiteLine(uint32_t azimuth);
  void SetupStopIndex() noexcept;
  // Обновление скана
  void UpdateScan(uint32_t azimuth);

  void Transform(csignal<double> &newSignal);

  void SetupSignal(csignal<double> &signal);

  // выполняем проверку на цвета на поподания в диапозон
  inline int CheckValColor(double val) noexcept;
  // Функция получения цвета
  // взависимости от диапозона данных
  QColor GetColor(double val);

  QColor GetColorView(double val) noexcept;

  // формируем вектор градиентов
  void CreateQGradientStops();

  // получаем длину сигнала для отрисовки
  int GetSizePlotSignal() noexcept;
  // устанавливаем значение яркости в отображении
  inline void SetValColor(int pos, double val);

  // Параметры модуля
  // вертикальный сдвиг сигнала
  int m_verticalShift = 0;
  // усиление сигнала
  int m_gain = 1;
  // шаг обновления сигнала
  int m_updateStep = 64;
  // номер текущего сигнала
  int m_currentFrame = 0;

  // режим обновления всего сигнала
  bool m_updateScan = false;
  // обновить все дорожки
  bool m_isUpdateAll = false;
  // режим цветного отображения
  bool m_isColorView = false;
  // отрисововать ли белую линию
  bool m_isWhiteLine = true;

  uint8_t m_coefStreatch = 1;

  // длина принимаемого сигнала
  size_t m_currentSignalSize = 0;
  // начальный индекс отображения данных
  size_t m_startIndexPlot = 0;
  // конечный индекс отображения данных
  size_t m_stopIndexPolot = 2047;

  // указатель на матрицу отображения
  std::vector<SectorBaseItem *> m_dataMatrix;

  // вектор градиентов для отображения
  QGradientStops m_gradientPoints;

  // время последнего обновления экрана
  std::chrono::high_resolution_clock::time_point m_lastTimeUpdate;

  // количество углов в развертке
  uint32_t m_coutAngel = 64;

  // Зеленый
  QColor m_color1 = QColor(76, 187, 23);
  // Желтый
  QColor m_color2 = QColor(248, 228, 115);
  // Красный
  QColor m_color3 = QColor(194, 24, 7);
  // Минимальное значение
  QColor m_colorMin = QColor(0, 0, 0);
  // Максимальное значение
  QColor m_colorMax = QColor(194, 24, 7);
};

#endif // SCANBUFFERSCENEMODULE_H
