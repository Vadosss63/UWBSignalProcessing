#ifndef LINECONNECT_H
#define LINECONNECT_H

#include <QGraphicsObject>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsScene>
#include <QMimeData>
#include <QDrag>
#include <QApplication>
#include <QBitmap>
#include "abstractblock.h"
#include "moduleconnectports.h"

enum statusConnect {outToIn = 0, inToOut, connects};

// класс выполняет графическое соединение в виде линий;
// содержит информацию о входном и выходном портах
class LineConnect : public QGraphicsObject
{
    Q_OBJECT
public:
    // Создание соединения от выходного в входному
    LineConnect(OPortBlock* fromNode);
    // Создание соединения от выходного в входному
    LineConnect(IPortBlock* toNode);

    ~LineConnect() override;
    LineConnect(const LineConnect& rhs) = delete;
    LineConnect& operator=(const LineConnect& rhs) = delete;

    // действия отрисовки линии
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

    // Действие при нажатии мыши
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // Действи при перемещении мыши
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    // Действие при отпускании мыши
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
    // Получение указателя на выходной порт
    OPortBlock* GetOutPort() const;
    // Получение указателя на входной порт
    IPortBlock* GetInPort() const;
    // Установка указателя выходного порта
    void SetPort(OPortBlock* port);
    // Установка указателя входного порта
    void SetPort(IPortBlock* port);
    // Получение статуса соединения
    statusConnect GetStatus();
    // Устанока статуса соединенния
    void SetStatus(statusConnect status);
    // установка стиля линии
    void SetPen(const QPen &pen);
    bool Connect();

signals:
    void MouseMove(QGraphicsSceneMouseEvent *event);
    // сигнал обновления всего содержимого
    void UpdateAll_sig();
public slots:
    // проверка наведения на входной порт блока
    void HoverEnterInPort(QGraphicsSceneMouseEvent *event);
    // проверка наведения на выходной порт блока
    void HoverEnterOutPort(QGraphicsSceneMouseEvent *event);
    // Действия при перемещении обьекта
    void TrackNodes();
    // Действия при перемещении обьекта
    void TrackNodes(const QPointF& p1, const QPointF& p2);
    // Обновление всего содержимого
    void UpdateAll();
private:
    void InitLine(QGraphicsObject* item, const QPointF& point);
    // Указатель на выходной порт
    OPortBlock* m_outPort;
    // Указатель на входной порт
    IPortBlock*  m_inPort;
    // Флаг соединения
    statusConnect m_flagConnect;
    // рисунок линии
    QVector<QLineF> m_lines;
    QRectF m_borderRect;
    // стиль отрисовки линии
    QPen m_pen;

};

// абстрактный класс выполняющий обработку действий и определяющий поведения отрисовки
class AbstractPortBlock: public QGraphicsObject
{
    Q_OBJECT
public:
    AbstractPortBlock(const QPoint &point, QGraphicsItem *parent);
    ~AbstractPortBlock() override;
    AbstractPortBlock(const AbstractPortBlock& rhs) = delete;
    AbstractPortBlock& operator=(const AbstractPortBlock& rhs) = delete;

    QPainterPath shape() const  override;
    // получение точки конца линии
    virtual QPointF GetPointLine() =  0;
    //отрисовка порта при наведении мыши
    void CreateImageHoverEnter();
    //отрисовка порта при уходе мыши
    void CreateImageHoverLeave();
    //получение указателя на линию
    LineConnect* GetLine() const;
    // установка указателя на линию
    void SetLine(LineConnect *Lines);
    // сброс указателя на линии
    virtual void LineReset();

    size_t GetId() const;
    void SetId(size_t id);
    void SetParentId(size_t parentId);
    size_t GetParentId() const;

    virtual void TryConnect() = 0;

public slots:
    // запуск перемещения
    void TrackNodes();

protected:
    // прорисовка линий портов
    virtual void DrawRect(QPainter *painter);
    // события обработки размеров обьекта
    QRectF boundingRect() const override;
    // события перересовки обета
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    // событи при нажатии мыши
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    // действие при наведении на область порта мышью
    void hoverEnterEvent(QGraphicsSceneHoverEvent*) override;
    // действие при выходе из область порта мыши
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*) override;

    void ConnectPorts(OPortBlock* outPortBlock, IPortBlock* inPortBlock);

private:
    // квадрат обьекта
    QRectF m_borderRect;
    // линия соединенния
    LineConnect* m_lines;
    // для отображения круга при подключении
    int sizeEllipse;
    // персональной номер для порта
    size_t m_id = 0;
    size_t m_parentId = 9999;///TODO
};

class OPortBlock: public AbstractPortBlock
{
    Q_OBJECT
public:
    OPortBlock(const QPoint& point, QGraphicsItem *parent);
    ~OPortBlock() override;
    virtual OPort* GetOutputPort() = 0;
};

class IPortBlock: public AbstractPortBlock
{
    Q_OBJECT
public:
    IPortBlock(const QPoint& point, QGraphicsItem *parent);
    ~IPortBlock() override;

    virtual IPort* GetInputPort() = 0;

};

// Класс отрисовки входного порта отвечеющий за соединения между элементами
class InPortBlock : public IPortBlock
{
public:
    InPortBlock(const QPoint& point, IPort* inputPort, QGraphicsItem *parent);
    ~InPortBlock() override;
    InPortBlock(const InPortBlock& rhs) = delete;
    InPortBlock& operator=(const InPortBlock& rhs) = delete;

    // получение точки конца линии
    QPointF GetPointLine() override;

    QPainterPath shape() const override;

    IPort* GetInputPort() override;

    void LineReset() override;

    void TryConnect() override;

protected:

    // события перересовки обета
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    // событи при нажатии мыши
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    // прорисовка линий портов
    void DrawRect(QPainter *painter) override;

private:
    // рисунок порта
    QLine inPuts;
    // ссылка на порт
    IPort* m_inputPort;
};

// Класс отрисовки выходного порта отвечеющий за соединения между элементами
class OutPortBlock : public OPortBlock
{
public:
    OutPortBlock(const QPoint& point, OPort* outputPort, QGraphicsItem *parent);
    ~OutPortBlock() override;
    OutPortBlock(const OutPortBlock& rhs) = delete;
    OutPortBlock& operator=(const OutPortBlock& rhs) = delete;

    // получение точки конца линии
    QPointF GetPointLine() override;

    QPainterPath shape() const override;

    OPort* GetOutputPort() override;
    // выполнение обнуления лини и отключение
    void LineReset() override;

    void TryConnect() override;

protected:
    // события перересовки обета
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    // событи при нажатии мыши
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    // прорисовка линий портов
    void DrawRect(QPainter *painter) override;

private:
    // рисунок порта
    QLine outPuts;
    // ссылка на порт
    OPort* m_outputPort;
};

#endif // LINECONNECT_H
