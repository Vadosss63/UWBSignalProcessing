#ifndef GRID_H
#define GRID_H

#include <QGraphicsPathItem>
#include <QLinearGradient>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#define M_PI 3.14159265358979323846

class SectorGrid: public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:

    SectorGrid(QGraphicsItem *parent = nullptr);
    ~SectorGrid() override = default;

    static qreal MaxVal_Y();
    static qreal MinVal_Y();
    static qreal MaxVal_X();
    // перевод значений [м] в пиксели
    static qreal Val_XToPixel(double val);
    static qreal Val_YToPixel(double val);

    // перевод значений [м] в пиксели
    static qreal Val_XToPixelLeftPanel(double val);
    static qreal Val_YToPixelLeftPanel(double val);

    const QColor& GetColorGrid() const;
    void SetColorGrid(const QColor& colorGrid);

    void ShowCurrentSpeed(double speed);

    static bool ContenesZoom(qreal y);

protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *) override;

    void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    QPainterPath shape() const override;

private:

    QRectF GetRect() const;
    void SetRect(const QRectF &GetRect);

    void CreateAxisX(QPainter* painter, QPainterPath& path);
    void CreateAxisY(QPainter* painter, QPainterPath& path);
    void CreateDangerTrack(double width, QPainter* painter);
    void CreateValue();
    void CreateLeftPanel();

    void DrawLeftPanel(QPainter* painter);
    // Создание зоны обзора
    QPainterPath CreateZoneLook();
    // Создание ближней зоны обзона
    QPainterPath CreateNearZoneLook();
    QPainterPath GetZoneLook(const double constVal) const;
    void IncShowRange();
    void DecShowRange();

    QGraphicsTextItem* m_value = nullptr;
    QGraphicsTextItem* m_valueSpeed = nullptr;

    // Максимальное занчение угла обзора дальней зоны
    double m_maxAngel = 7.0;
    // Количество линий на дискретах
    int m_numTicks = 6;  
    // Цвет сетки
    QColor m_colorGrid = QColor("#000000");
    // Квадрат позиционирования секторов
    QRectF m_rectGrid;

    // максимальное значние по y (м)
    static qreal m_maxVal_YZoom;
    // минимальная значение по у (м)
    static qreal m_minVal_YZoom;

    // максимальное значение по X (м)
    constexpr static const qreal m_maxVal_XZoom = 15;
    // максимальное значние по y (м)
    constexpr static const qreal m_maxVal_YPanel = 150;
    // максимальное значение по X (м)
    constexpr static const qreal m_maxVal_XPanel = 15;

    constexpr static const qreal m_width = 1024;
    constexpr static const qreal m_height = 1024;
    constexpr static const qreal m_widthLeftPanel = 1024 / 5;
    static QRectF m_leftPanel;


};

#endif // GRID_H
