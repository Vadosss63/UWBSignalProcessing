#include <utility>
#include "plotter.h"

QColor DrawCurver::m_colorForIds[] { Qt::white, Qt::red, Qt::yellow, Qt::magenta, Qt::cyan, Qt::blue};

DrawCurver::DrawCurver(QWidget *parent, int countChenels) :
    QWidget(parent), m_curves(countChenels), m_type(typeValue)
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
    SetColorGrid(Qt::yellow);
}

void DrawCurver::SetPlotSetting(const PlotSetting &setting)
{
    m_zoomStack.clear();
    m_zoomStack.append(setting);
    m_curZoom = 0;
    m_zoomInButton->hide();
    m_zoomOutButton->hide();
    refreshPixmap();
}

QSize DrawCurver::minimumSizeHint() const
{
    return QSize(6 * MARGIN, 4 * MARGIN);
}

QSize DrawCurver::sizeHint() const
{
    return QSize(12 * MARGIN, 8 * MARGIN);
}

void DrawCurver::SetSettindDisplay(TypeX type, int val, QString name)
{
    m_type = type;
    m_labelX = val;
    m_labelXName = std::move(name);
}

void DrawCurver::ZoomIn()
{
    if(m_curZoom < m_zoomStack.count() - 1)
    {
        ++m_curZoom;
        m_zoomInButton->setEnabled(m_curZoom < m_zoomStack.count() - 1);
        m_zoomOutButton->setEnabled(true);
        m_zoomOutButton->show();
        refreshPixmap();
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
        refreshPixmap();
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
        painter.setPen(m_colorGrid.light());
        painter.drawRect(m_rubberBandRect.normalized().adjusted(0, 0, -1, -1));
    }

    if(hasFocus())
    {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = m_colorGrid.dark();
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }
}

void DrawCurver::resizeEvent(QResizeEvent */*event*/)
{
    int x = width() - (m_zoomInButton->width() + m_zoomOutButton->width() + 10);
    m_zoomInButton->move(x, 5);
    m_zoomOutButton->move(x + m_zoomInButton->width() + 5, 5);
    refreshPixmap();
}

void DrawCurver::mousePressEvent(QMouseEvent *event)
{
    QRect rect(MARGIN, MARGIN, width() - 2 * MARGIN, height() - 2 * MARGIN);
    if(event->button() == Qt::LeftButton)
    {
        if(rect.contains(event->pos()))
        {
            m_rubberBandIsShown = true;
            m_rubberBandRect.setTopLeft(event->pos());
            m_rubberBandRect.setBottomRight(event->pos());
            updateRubberBandRegion();
            setCursor(Qt::CrossCursor);
        }
    }
}

void DrawCurver::mouseMoveEvent(QMouseEvent *event)
{
    if(m_rubberBandIsShown)
    {
        updateRubberBandRegion();
        m_rubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
    }
}

void DrawCurver::mouseReleaseEvent(QMouseEvent *event)
{
    if((event->button() == Qt::LeftButton) && m_rubberBandIsShown)
    {
        m_rubberBandIsShown = false;
        updateRubberBandRegion();
        unsetCursor();
        QRect rect  = m_rubberBandRect.normalized();
        if(rect.width() < 4 || rect.height() < 4)
            return;
        rect.translate(-MARGIN, -MARGIN);
        PlotSetting prevSettings = m_zoomStack[m_curZoom];
        PlotSetting setting;
        double dx = prevSettings.SpanX()/(width() - 2 * MARGIN);
        double dy = prevSettings.SpanY()/(height() - 2 * MARGIN);
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
        refreshPixmap();
        break;
    case Qt::Key_Right:
        m_zoomStack[m_curZoom].Scroll(+1, 0);
        refreshPixmap();
        break;
    case Qt::Key_Down:
        m_zoomStack[m_curZoom].Scroll(0, -1);
        refreshPixmap();
        break;
    case Qt::Key_Up:
        m_zoomStack[m_curZoom].Scroll(0, +1);
        refreshPixmap();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void DrawCurver::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees / 15;
    if(event->orientation() == Qt::Horizontal)
    {
        m_zoomStack[m_curZoom].Scroll(numTicks, 0);
    } else {
        m_zoomStack[m_curZoom].Scroll(0, numTicks);
    }
    refreshPixmap();
}

void DrawCurver::updateRubberBandRegion()
{
    QRect rect = m_rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

void DrawCurver::refreshPixmap()
{
    m_pixmap = QPixmap(size());
    m_pixmap.fill(m_colorBackground);
    QPainter painter(&m_pixmap);
    painter.initFrom(this);
    drawGrig(&painter);
    drawCurves(&painter);
    update();
}

void DrawCurver::drawGrig(QPainter *painter)
{
    QRect rect(MARGIN, MARGIN, width() - 2 * MARGIN, height() -  2 * MARGIN);
    if(!rect.isValid())
        return;

    PlotSetting settings = m_zoomStack[m_curZoom];
    QPen quiteDark = m_colorGrid;
    QPen light = m_colorGrid.light();
    QString lableXText("Отсчеты");

    for (int i = 0; i < settings.numXTicks + 1; ++i) {
        int x = rect.left() + (i * (rect.width()-1)/ settings.numXTicks);
        double label = settings.minX + (i * settings.SpanX()/settings.numXTicks);
        if(m_type == typeFrequency)
        {
            label = label * m_labelX/(m_curves.first().size());
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
        painter->drawText(rect.left() - MARGIN, y - 10,  MARGIN -5, 20,
                          Qt::AlignRight|Qt::AlignVCenter,
                          QString::number(label));

    }
    painter->drawRect(rect.adjusted(0,0,-1,-1));
}

void DrawCurver::drawCurves(QPainter *painter)
{
    PlotSetting settings = m_zoomStack[m_curZoom];
    QRect rect(MARGIN, MARGIN,
               width() - 2 * MARGIN, height() - 2 * MARGIN);

    if(!rect.isValid())
        return;

    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));
    unsigned char i = 0;
    for(auto& curve : m_curves)
    {
        QPolygonF polyline(curve.count());

        for (int j = 0; j < curve.count(); ++j) {
            double dx = curve[j].x() - settings.minX;
            double dy = curve[j].y() - settings.minY;
            double x = rect.left() + (dx * (rect.width() - 1) / settings.SpanX());
            double y = rect.bottom() - (dy * (rect.height() - 1) / settings.SpanY());
            polyline[j] = QPointF(x, y);
        }

        painter->setPen(m_colorForIds[i++]);
        painter->drawPolyline(polyline);
    }
}

void DrawCurver::ChangeSizeData(int newSize)
{
    for (auto& curve : m_curves) {
        if(curve.size() != newSize)
        {
            curve.resize(newSize);
            int x = 0;
            for(QPointF& value : curve) {
                value.setX(x);
                x++;
            }
        }
    }
}

size_t DrawCurver::GetSizeData()
{
    return m_curves.size();
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

void PlotSetting::AdjustAxis(double &min, double &max, int &numTicks) const
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

Plotter::Plotter(CSignalPlotter *scopeBufferModule, QWidget *parent): DrawCurver(parent),
    m_scopeBufferModule(scopeBufferModule)
{
    connect(scopeBufferModule, SIGNAL(BufferUpdated()), this, SLOT(CreateData()));
}

void Plotter::CreateData()
{
    csignal<double> sig;

    if(!m_scopeBufferModule->PopSignal(sig))
        return;

    SetCurveData(sig, 0);
}

PlotterComplex::PlotterComplex(CSignalComplexPlotter* scopeBufferModule, QWidget* parent): DrawCurver(parent, 2),
    m_scopeBufferModule(scopeBufferModule)
{
    connect(scopeBufferModule, SIGNAL(BufferUpdated()), this, SLOT(CreateData()));
}

void PlotterComplex::CreateData()
{
    std::pair<csignal<double>, csignal<double>> sig;

    if(!m_scopeBufferModule->PopSignal(sig))
        return;

    SetCurveData(sig);
}
