#ifndef FFTPLOTER_H
#define FFTPLOTER_H
#include <QKeyEvent>
#include <QMap>
#include <QMapIterator>
#include <QMouseEvent>
#include <QPixmap>
#include <QStyleOptionFocusRect>
#include <QStylePainter>
#include <QToolButton>
#include <QVector>
#include <QWidget>
#include <math.h>

class QToolButton;
class PlotSetting;

// тип оси x
enum TypeX { typeValue = 0, typeTime, typeFrequency, typeRange };

class DrawCurver : public QWidget {
  Q_OBJECT
public:
  DrawCurver(QWidget *parent = nullptr);
  // установка настройки отображения
  void SetPlotSetting(const PlotSetting &setting);
  void SetData(const QVector<QPointF> &data);
  // установка данных
  template <typename T> void SetCurveData(const T &data);

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  void SetSettindDisplay(TypeX type, int val, QString name = QString());

  QVector<QPointF> &GetCuver();
  void RefreshPixmap();
  void ChangeSizeData(size_t newSize);

public slots:

  void ZoomIn();
  void ZoomOut();
  // цвет передается в формате ""
  void SetColorBackground(QColor color);
  void SetColorGrid(QColor color);

protected:
  void paintEvent(QPaintEvent *);
  void resizeEvent(QResizeEvent *);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  void UpdateRubberBandRegion();
  void DrawGrig(QPainter *painter);
  void DrawCurves(QPainter *painter);
  size_t GetSizeData();

  enum { Margin = 70 };
  QToolButton *m_zoomInButton;
  QToolButton *m_zoomOutButton;
  QVector<QPointF> m_curve;
  QVector<PlotSetting> m_zoomStack;
  double m_labelX = 1;
  int m_curZoom;
  bool m_rubberBandIsShown;
  QRect m_rubberBandRect;
  QPixmap m_pixmap;
  QString m_labelXName;
  TypeX m_type;
  QColor m_colorBackground;
  QColor m_colorGrid;
  static QColor m_colorForIds[6];
};

// устанивливает настройки по умолчанию
class PlotSetting {

public:
  PlotSetting();
  PlotSetting(double max_X, double max_Y, double num_XTicks = 10,
              double num_YTicks = 10);
  PlotSetting(double max_X, double max_Y, double min_X, double min_Y,
              double num_XTicks = 10, double num_YTicks = 10);
  void Scroll(int dx, int dy);
  void Adjust();
  double SpanX() const { return maxX - minX; }
  double SpanY() const { return maxY - minY; }
  double minX;
  double minY;
  double maxX;
  double maxY;
  int numYTicks;
  int numXTicks;

private:
  static void AdjustAxis(double &min, double &max, int &numTicks);
};

#endif // FFTPLOTER_H
