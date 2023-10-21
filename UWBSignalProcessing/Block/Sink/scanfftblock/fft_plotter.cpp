#include "fft_plotter.h"

#include <utility>

QColor DrawCurver::m_colorForIds[] { Qt::white, Qt::red, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow};

DrawCurver::DrawCurver(QWidget *parent) :
    QWidget(parent), m_type(typeValue)
{
    setBackgroundRole(QPalette::Midlight);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    m_rubberBandIsShown = false;
    m_zoomInButton = new QToolButton(this);
    m_zoomInButton->setIcon(QIcon(":/images/zoom-in.svg"));
    m_zoomInButton->adjustSize();
    connect(m_zoomInButton, SIGNAL(clicked()), this, SLOT(ZoomIn()));

    m_zoomOutButton = new QToolButton(this);
    m_zoomOutButton->setIcon(QIcon(":/images/zoom-out.svg"));
    m_zoomOutButton->adjustSize();
    connect(m_zoomOutButton, SIGNAL(clicked()), this, SLOT(ZoomOut()));
    SetPlotSetting(PlotSetting());
    SetColorBackground(Qt::black);
    SetColorGrid(QColor("#D0D0D0"));
}

void DrawCurver::SetPlotSetting(const PlotSetting &setting)
{
    m_zoomStack.clear();
    m_zoomStack.append(setting);
    m_curZoom = 0;
    m_zoomInButton->hide();
    m_zoomOutButton->hide();
    RefreshPixmap();
}

void DrawCurver::SetData(const QVector<QPointF> &data)
{
    m_curve = data;
    RefreshPixmap();
}

template <typename T> void DrawCurver::SetCurveData(const T& data)
{
    ChangeSizeData(data.size());

    for (int i = 0; i < m_curve.size(); ++i)
        m_curve[i].setY(data[i]);

    RefreshPixmap();
}

QSize DrawCurver::minimumSizeHint() const
{
    return QSize(6 * Margin, 4 * Margin);
}

QSize DrawCurver::sizeHint() const
{
    return QSize(12 * Margin, 8 * Margin);
}

void DrawCurver::SetSettindDisplay(TypeX type, int val, QString name)
{
    m_type = type;
    m_labelX = val;
    m_labelXName = std::move(name);
}

QVector<QPointF>& DrawCurver::GetCuver()
{
    return m_curve;
}

void DrawCurver::ZoomIn()
{
    if(m_curZoom < m_zoomStack.count() - 1)
    {
        ++m_curZoom;
        m_zoomInButton->setEnabled(m_curZoom < m_zoomStack.count() - 1);
        m_zoomOutButton->setEnabled(true);
        m_zoomOutButton->show();
        RefreshPixmap();
    }
}

void DrawCurver::ZoomOut()
{
    if(m_curZoom > 0)
    {
        --m_curZoom;
        m_zoomOutButton->setEnabled(m_curZoom > 0);
        m_zoomInButton->setEnabled(true);
        m_zoomInButton->show();
        RefreshPixmap();
    }
}

void DrawCurver::SetColorBackground(QColor color)
{
    m_colorBackground = std::move(color);
}

void DrawCurver::SetColorGrid(QColor color)
{
    m_colorGrid = std::move(color);
}

void DrawCurver::paintEvent(QPaintEvent * /*event*/)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, m_pixmap);
    if(m_rubberBandIsShown)
    {
        painter.setPen(m_colorGrid.lighter());
        painter.drawRect(m_rubberBandRect.normalized().adjusted(0, 0, -1, -1));
    }

    if(hasFocus())
    {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = m_colorGrid.darker();
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }
}

void DrawCurver::resizeEvent(QResizeEvent */*event*/)
{
    int x = width() - (m_zoomInButton->width() + m_zoomOutButton->width() + 10);
    m_zoomInButton->move(x, 5);
    m_zoomOutButton->move(x + m_zoomInButton->width() + 5, 5);
    RefreshPixmap();
}

void DrawCurver::mousePressEvent(QMouseEvent *event)
{
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    if(event->button() == Qt::LeftButton)
    {
        if(rect.contains(event->pos()))
        {
            m_rubberBandIsShown = true;
            m_rubberBandRect.setTopLeft(event->pos());
            m_rubberBandRect.setBottomRight(event->pos());
            UpdateRubberBandRegion();
            setCursor(Qt::CrossCursor);
        }
    }
}

void DrawCurver::mouseMoveEvent(QMouseEvent *event)
{
    if(m_rubberBandIsShown)
    {
        UpdateRubberBandRegion();
        m_rubberBandRect.setBottomRight(event->pos());
        UpdateRubberBandRegion();
    }
}

void DrawCurver::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->button() == Qt::LeftButton) && m_rubberBandIsShown)
    {
        m_rubberBandIsShown = false;
        UpdateRubberBandRegion();
        unsetCursor();
        QRect rect  = m_rubberBandRect.normalized();
        if(rect.width() < 4 || rect.height() < 4)
            return;
        rect.translate(-Margin, -Margin);
        PlotSetting prevSettings = m_zoomStack[m_curZoom];
        PlotSetting setting;
        double dx = prevSettings.SpanX()/(width() - 2 * Margin);
        double dy = prevSettings.SpanY()/(height() - 2 * Margin);
        setting.minX = prevSettings.minX + dx * rect.left();
        setting.maxX = prevSettings.minX + dx * rect.right();
        setting.minY = prevSettings.maxY - dy * rect.bottom();
        setting.maxY = prevSettings.maxY - dy * rect.top();
        setting.Adjust();
        m_zoomStack.resize(m_curZoom + 1);
        m_zoomStack.append(setting);
        ZoomIn();
    }
}

void DrawCurver::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        ZoomIn();
        break;
    case Qt::Key_Minus:
        ZoomOut();
        break;
    case Qt::Key_Left:
        m_zoomStack[m_curZoom].Scroll(-1, 0);
        RefreshPixmap();
        break;
    case Qt::Key_Right:
        m_zoomStack[m_curZoom].Scroll(+1, 0);
        RefreshPixmap();
        break;
    case Qt::Key_Down:
        m_zoomStack[m_curZoom].Scroll(0, -1);
        RefreshPixmap();
        break;
    case Qt::Key_Up:
        m_zoomStack[m_curZoom].Scroll(0, +1);
        RefreshPixmap();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void DrawCurver::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->angleDelta().y() / 8;
    int numTicks = numDegrees / 15;
    if (event->angleDelta().x() != 0)
    {
        m_zoomStack[m_curZoom].Scroll(numTicks, 0);
    } else {
        m_zoomStack[m_curZoom].Scroll(0, numTicks);
    }
    RefreshPixmap();
}

void DrawCurver::UpdateRubberBandRegion()
{
    QRect rect = m_rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

void DrawCurver::RefreshPixmap()
{
    m_pixmap = QPixmap(size());
    m_pixmap.fill(m_colorBackground);
    QPainter painter(&m_pixmap);
    painter.begin(this);
    DrawGrig(&painter);
    DrawCurves(&painter);
    update();
}

void DrawCurver::DrawGrig(QPainter *painter)
{
    QRect rect(Margin, Margin, width() - 2 * Margin, height() -  2 * Margin);
    if(!rect.isValid())
        return;

    PlotSetting settings = m_zoomStack[m_curZoom];
    QPen quiteDark = m_colorGrid;
    quiteDark.setStyle(Qt::DashLine);
    quiteDark.setWidthF(0.5);
    QPen light = m_colorGrid.lighter();
    QString lableXText("Отсчеты");

    for (int i = 0; i < settings.numXTicks + 1; ++i) {
        int x = rect.left() + (i * (rect.width()-1)/ settings.numXTicks);
        double label = settings.minX + (i * settings.SpanX()/settings.numXTicks);
        if(m_type == typeFrequency)
        {
            label = label * m_labelX/(m_curve.size());
            lableXText = "Частота";
        }
        else if(m_type == typeTime)
        {
            label = label * m_labelX;
            lableXText = "Время";

        }else if (m_type == typeRange) {
            label = (label * m_labelX) / 100;
            lableXText = "Дальность";
        }

        painter->setPen(quiteDark);
        painter->drawLine(x, rect.top(), x , rect.bottom());
        painter->setPen(light);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        painter->drawText(x - 50, rect.bottom() + 5, 100, 20, Qt::AlignHCenter|Qt::AlignTop,
                          QString::number(label));

    }

    painter->drawText(rect.center().x() - 50, rect.bottom() + 30, 100, 20, Qt::AlignHCenter|Qt::AlignTop,
                      lableXText);

    for (int j = 0; j < settings.numYTicks + 1; ++j) {
        int y = rect.bottom() - (j * (rect.height() - 1)
                                 /settings.numYTicks);
        double label = settings.minY + (j * settings.SpanY()
                                        /settings.numYTicks);

        painter->setPen(quiteDark);
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(light);
        painter->drawLine(rect.left() - 5,y, rect.left(), y );
        painter->drawText(rect.left() - Margin, y - 10,  Margin -5, 20,
                          Qt::AlignRight|Qt::AlignVCenter,
                          QString::number(label));

    }
    painter->drawRect(rect.adjusted(0,0,-1,-1));
}

void DrawCurver::DrawCurves(QPainter *painter)
{
    PlotSetting settings = m_zoomStack[m_curZoom];
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);

    if(!rect.isValid())
        return;

    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

    QPolygonF polyline(m_curve.count());

    for (int j = 0; j < m_curve.count(); ++j) {
        double dx = m_curve[j].x() - settings.minX;
        double dy = m_curve[j].y() - settings.minY;
        double x = rect.left() + (dx * (rect.width() - 1) / settings.SpanX());
        double y = rect.bottom() - (dy * (rect.height() - 1) / settings.SpanY());
        polyline[j] = QPointF(x, y);
    }

    painter->setPen(m_colorForIds[0]);
    painter->drawPolyline(polyline);
}

void DrawCurver::ChangeSizeData(size_t newSize)
{
    if(static_cast<size_t>(m_curve.size()) != newSize)
    {
        m_curve.resize(newSize);
        int x = 0;
        for(QPointF& value : m_curve) {
            value.setX(x);
            x++;
        }
        PlotSetting setting =  m_zoomStack.first();
        setting.maxX = newSize;
        setting.maxY = 2048;
        SetPlotSetting(setting);
    }
}

size_t DrawCurver::GetSizeData()
{
    return m_curve.size();
}

PlotSetting::PlotSetting()
{
    minX = 0.0;
    maxX = 2100.0;
    numXTicks = 10;

    minY = -1100.0;
    maxY = 1100.0;
    numYTicks = 10;
}

PlotSetting::PlotSetting(double max_X, double max_Y, double num_XTicks, double num_YTicks)
{
    minX = 0;
    maxX = max_X;
    numXTicks = num_XTicks;
    minY = -max_Y;
    maxY = max_Y;
    numYTicks = num_YTicks;
}

PlotSetting::PlotSetting(double max_X, double max_Y, double min_X, double min_Y, double num_XTicks, double num_YTicks)
{
    minX = min_X;
    maxX = max_X;
    numXTicks = num_XTicks;
    minY = min_Y;
    maxY = max_Y;
    numYTicks = num_YTicks;
}

void PlotSetting::Scroll(int dx, int dy)
{
    double stepX = SpanX() / numXTicks;
    minX += dx * stepX;
    maxX += dx * stepX;

    double stepY = SpanY() / numYTicks;
    minY += dy * stepY;
    maxY += dy * stepY;

}

void PlotSetting::Adjust()
{
    AdjustAxis(minX, maxX, numXTicks);
    AdjustAxis(minY, maxY, numYTicks);
}

void PlotSetting::AdjustAxis(double &min, double &max, int &numTicks)
{
    const int MinTicks = 4;
    double grossStep = (max - min)/ MinTicks;
    double step = std::pow(10.0, std::floor(std::log10(grossStep)));

    if(5 * step < grossStep)
        step *= 5;
    else if(2 * step < grossStep )
        step *= 2;

    numTicks = int(std::ceil(max / step) - std::floor(min / step));
    if(numTicks < MinTicks)
        numTicks = MinTicks;

    min = std::floor(min / step) * step;
    max = std::ceil(max / step) * step;
}
