#ifndef PLOTTER_H
#define PLOTTER_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QToolButton>
#include <QStylePainter>
#include <QStyleOptionFocusRect>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMapIterator>
#include "plotter_interface.h"

class QToolButton;
class PlotSetting;
// тип оси x
enum TypeX
{
    typeValue = 0,
    typeTime,
    typeFrequency,
    typeRange
};

class DrawCurver: public QWidget
{
    Q_OBJECT
public:
    DrawCurver(QWidget *parent = nullptr, int countChenels = 1);
    // установка настройки отображения
    void SetPlotSetting(const PlotSetting &setting);
    // установка данных
    template <typename T> void SetCurveData(const T& data, int index);
    template <typename T> void SetCurveData(const std::pair<T, T>& data);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void SetSettindDisplay(TypeX type, int val, QString name = QString());

public slots:
    void ZoomIn();
    void ZoomOut();
    // цвет передается в формате ""
    void SetColorBackground(QColor color);
    void SetColorGrid(QColor color);

protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void updateRubberBandRegion();
    void refreshPixmap();
    void drawGrig(QPainter* painter);
    void drawCurves(QPainter* painter);
    void ChangeSizeData(int newSize);
    size_t GetSizeData();
    QToolButton* m_zoomInButton;
    QToolButton* m_zoomOutButton;
    QVector<QVector<QPointF>> m_curves;
    QVector<PlotSetting> m_zoomStack;
    QRect m_rubberBandRect;
    QPixmap m_pixmap;
    QColor m_colorBackground;
    QColor m_colorGrid;
    double m_labelX = 1;
    int m_curZoom;
    TypeX m_type;
    QString m_labelXName;
    bool m_rubberBandIsShown;
    static QColor m_colorForIds[6];
    constexpr static const int MARGIN = 70;
};

class Plotter: public DrawCurver
{
    Q_OBJECT
public:
    Plotter(CSignalPlotter* scopeBufferModule, QWidget *parent = nullptr);
    ~Plotter() = default;
public slots:

    void CreateData(); // слот обработки входящих csignal

private:
    CSignalPlotter* m_scopeBufferModule;
};

class PlotterComplex: public DrawCurver
{
    Q_OBJECT
public:
    PlotterComplex(CSignalComplexPlotter* scopeBufferModule, QWidget *parent = nullptr);
    ~PlotterComplex() = default;
public slots:

    void CreateData(); // слот обработки входящих csignal

private:
    CSignalComplexPlotter* m_scopeBufferModule;
};

// устанивливает настройки по умолчанию
class PlotSetting
{
public:
    PlotSetting();
    PlotSetting(double max_X, double max_Y, double num_XTicks = 10, double num_YTicks = 10);
    PlotSetting(double max_X, double max_Y, double min_X, double min_Y, double num_XTicks = 10, double num_YTicks = 10);
    void Scroll(int dx, int dy);
    void Adjust();
    double SpanX() const {return maxX - minX;}
    double SpanY() const {return maxY - minY;}
    double minX;
    double minY;
    double maxX;
    double maxY;
    int numYTicks;
    int numXTicks;
private:
    void AdjustAxis(double& min, double& max, int& numTicks) const;
};

template <typename T> void DrawCurver::SetCurveData(const T& data, int index)
{
    ChangeSizeData(data.size());

    for (size_t i = 0; i < data.size(); ++i)
        m_curves[index][i].setY(data[i]);

    refreshPixmap();
}

template <typename T> void DrawCurver::SetCurveData(const std::pair<T, T>& data)
{
    ChangeSizeData(data.first.size());

    for (size_t i = 0; i < data.first.size(); ++i)
    {
        m_curves[0][i].setY(data.first[i]);
        m_curves[1][i].setY(data.second[i]);
    }
    refreshPixmap();
}
#endif // PLOTTER_H
