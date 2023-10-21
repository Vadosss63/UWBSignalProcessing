#include "lineconnect.h"

LineConnect::LineConnect(OPortBlock *fromNode)
    : QGraphicsObject(), m_outPort(fromNode), m_inPort(nullptr), m_lines(3),
      m_borderRect() {
  SetStatus(outToIn);
  InitLine(fromNode, m_outPort->mapToScene(m_outPort->GetPointLine()));
}

LineConnect::LineConnect(IPortBlock *toNode)
    : QGraphicsObject(), m_outPort(nullptr), m_inPort(toNode), m_lines(3),
      m_borderRect() {
  SetStatus(inToOut);
  InitLine(toNode, m_inPort->mapToScene(m_inPort->GetPointLine()));
}

LineConnect::~LineConnect() {
  // очистка указателя на порт
  if (m_inPort)
    m_inPort->LineReset();

  if (m_outPort)
    m_outPort->LineReset();

  m_inPort = nullptr;
  m_outPort = nullptr;
}

QRectF LineConnect::boundingRect() const {
  int Padding = 40;
  QRectF rect(m_borderRect);
  rect.adjust(-Padding, -Padding, +Padding, +Padding);
  return rect;
}

QPainterPath LineConnect::shape() const { return QPainterPath(); }

void LineConnect::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                        QWidget *) {
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(m_pen);
  painter->drawLines(m_lines);
  //    painter->setPen(Qt::red);
  //    painter->drawRect(boundingRect());
}

void LineConnect::TrackNodes() {
  QPointF p1(m_outPort->mapToScene(m_outPort->GetPointLine()));
  QPointF p2(m_inPort->mapToScene(m_inPort->GetPointLine()));
  TrackNodes(p1, p2);
}
#include <iostream>
void LineConnect::TrackNodes(const QPointF &p1, const QPointF &p2) {

  if (p1.x() > p2.x()) {
    if (p1.y() < p2.y()) {
      m_lines[0].setPoints(p1,
                           QPointF(p1.x(), p1.y() + m_borderRect.height() / 2));

      m_lines[1].setPoints(QPointF(p1.x(), p1.y() + m_borderRect.height() / 2),
                           QPointF(p2.x(), p1.y() + m_borderRect.height() / 2));

      m_lines[2].setPoints(QPointF(p2.x(), p1.y() + m_borderRect.height() / 2),
                           p2);

      m_borderRect = QRectF(QPointF(p2.x(), p1.y()), QPointF(p1.x(), p2.y()));
    } else {

      m_lines[0].setPoints(p1,
                           QPointF(p1.x(), p2.y() + m_borderRect.height() / 2));

      m_lines[1].setPoints(QPointF(p1.x(), p2.y() + m_borderRect.height() / 2),
                           QPointF(p2.x(), p2.y() + m_borderRect.height() / 2));
      m_lines[2].setPoints(QPointF(p2.x(), p2.y() + m_borderRect.height() / 2),
                           p2);
      m_borderRect = QRectF(p2, p1);
    }

  } else {
    m_lines[0].setPoints(p1,
                         QPointF(p1.x() + m_borderRect.width() / 2, p1.y()));

    m_lines[1].setPoints(QPointF(p1.x() + m_borderRect.width() / 2, p1.y()),
                         QPointF(p1.x() + m_borderRect.width() / 2, p2.y()));

    m_lines[2].setPoints(QPointF(p1.x() + m_borderRect.width() / 2, p2.y()),
                         p2);

    if (p1.y() > p2.y())
      m_borderRect = QRectF(QPointF(p1.x(), p2.y()), QPointF(p2.x(), p1.y()));
    else
      m_borderRect = QRectF(p1, p2);
  }

  //    std::cout << __FUNCTION__ << std::endl;

  emit UpdateAll_sig();
}

void LineConnect::UpdateAll() {
  if (scene())
    scene()->update();
}

void LineConnect::SetPort(OPortBlock *port) {
  if (m_outPort == port)
    return;

  if (m_outPort)
    m_outPort->CreateImageHoverLeave();
  m_outPort = port;
}

void LineConnect::SetPort(IPortBlock *port) {
  if (m_inPort == port)
    return;

  if (m_inPort)
    m_inPort->CreateImageHoverLeave();
  m_inPort = port;
}

void LineConnect::SetPen(const QPen &pen) { m_pen = pen; }

bool LineConnect::Connect() {
  if (!m_inPort->GetInputPort()->Connect(m_outPort->GetOutputPort())) {
    delete this;
    return false;
  }
  SetStatus(connects);
  TrackNodes();
  ungrabMouse();
  return true;
}

OPortBlock *LineConnect::GetOutPort() const { return m_outPort; }

IPortBlock *LineConnect::GetInPort() const { return m_inPort; }

void LineConnect::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mousePressEvent(event);
}

void LineConnect::HoverEnterInPort(QGraphicsSceneMouseEvent *event) {
  TrackNodes(m_outPort->mapToScene(m_outPort->GetPointLine()),
             event->scenePos());
  if (scene()->itemAt(event->scenePos(), QTransform())) {
    IPortBlock *other = dynamic_cast<IPortBlock *>(
        scene()->itemAt(event->scenePos(), QTransform()));
    // если порт есть, то отображаем на него наведения
    if (other)
      if (!other->GetLine()) {
        other->CreateImageHoverEnter();
        SetPort(other);
      }
  } else if (m_inPort) {
    m_inPort->CreateImageHoverLeave();
    m_inPort = nullptr;
  }
}

void LineConnect::HoverEnterOutPort(QGraphicsSceneMouseEvent *event) {
  TrackNodes(event->scenePos(), m_inPort->mapToScene(m_inPort->GetPointLine()));
  if (scene()->itemAt(event->scenePos(), QTransform())) {
    OPortBlock *other = dynamic_cast<OPortBlock *>(
        scene()->itemAt(event->scenePos(), QTransform()));
    if (other)
      if (!other->GetLine()) {
        other->CreateImageHoverEnter();
        SetPort(other);
      }
  } else if (m_outPort) {
    m_outPort->CreateImageHoverLeave();
    m_outPort = nullptr;
  }
}

void LineConnect::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  emit MouseMove(event);
}

void LineConnect::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
  switch (m_flagConnect) {
  case outToIn: {
    if (m_inPort)
      if (!m_inPort->GetLine()) {
        m_inPort->SetLine(this);
        break;
      }
    delete this;
    return;
  }
  case inToOut: {
    if (m_outPort)
      if (!m_outPort->GetLine()) {
        m_outPort->SetLine(this);
        break;
      }
    delete this;
    return;
  }
  default:
    return;
  }
  SetStatus(connects);
  if (!m_inPort->GetInputPort()->Connect(m_outPort->GetOutputPort())) {
    delete this;
    return;
  }
  TrackNodes();
  ungrabMouse();
}

statusConnect LineConnect::GetStatus() { return m_flagConnect; }

void LineConnect::SetStatus(statusConnect status) {
  switch (status) {
  case outToIn:
    connect(this, SIGNAL(MouseMove(QGraphicsSceneMouseEvent *)),
            SLOT(HoverEnterInPort(QGraphicsSceneMouseEvent *)));
    break;
  case inToOut:
    connect(this, SIGNAL(MouseMove(QGraphicsSceneMouseEvent *)),
            SLOT(HoverEnterOutPort(QGraphicsSceneMouseEvent *)));
    break;
  case connects: {
    if (m_flagConnect == outToIn)
      disconnect(this, SIGNAL(MouseMove(QGraphicsSceneMouseEvent *)), this,
                 SLOT(HoverEnterInPort(QGraphicsSceneMouseEvent *)));
    if (m_flagConnect == inToOut)
      disconnect(this, SIGNAL(MouseMove(QGraphicsSceneMouseEvent *)), this,
                 SLOT(HoverEnterOutPort(QGraphicsSceneMouseEvent *)));
  } break;
  }
  m_flagConnect = status;
}

void LineConnect::InitLine(QGraphicsObject *item, const QPointF &point) {
  if (item->scene()) {
    item->scene()->addItem(this);
    connect(this, SIGNAL(UpdateAll_sig()), SLOT(UpdateAll()));
    setZValue(1);
    setFlags(ItemIsMovable);
    grabMouse();
    m_lines[0] = QLineF(point, point);
    m_lines[1] = QLineF(point, point);
    m_lines[2] = QLineF(point, point);
    //        m_pen.setJoinStyle(Qt::RoundJoin);
  } else
    delete this;
}

AbstractPortBlock::AbstractPortBlock(const QPoint &point, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_lines(nullptr), sizeEllipse(1) {
  setZValue(1);
  setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
  this->setAcceptHoverEvents(true);
  m_borderRect = QRect(point.x(), point.y(), 24, 12);
}

AbstractPortBlock::~AbstractPortBlock() { delete m_lines; }

QRectF AbstractPortBlock::boundingRect() const { return m_borderRect; }

void AbstractPortBlock::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *, QWidget *) {
  DrawRect(painter);
}

void AbstractPortBlock::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
  CreateImageHoverEnter();
}

QPainterPath AbstractPortBlock::shape() const {
  QPainterPath path;
  return path;
}

void AbstractPortBlock::CreateImageHoverEnter() {
  sizeEllipse = 5;
  update();
}

void AbstractPortBlock::CreateImageHoverLeave() {
  sizeEllipse = 1;
  update();
}

void AbstractPortBlock::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
  CreateImageHoverLeave();
}

void AbstractPortBlock::ConnectPorts(OPortBlock *outPortBlock,
                                     IPortBlock *inPortBlock) {
  LineConnect *link = new LineConnect(outPortBlock);

  link->SetPort(inPortBlock);
  inPortBlock->SetLine(link);
  outPortBlock->SetLine(link);
  link->Connect();
}

size_t AbstractPortBlock::GetId() const { return m_id; }

void AbstractPortBlock::SetId(size_t id) { m_id = id; }

void AbstractPortBlock::SetParentId(size_t parentId) { m_parentId = parentId; }

size_t AbstractPortBlock::GetParentId() const { return m_parentId; }

void AbstractPortBlock::mousePressEvent(QGraphicsSceneMouseEvent *) {}

void AbstractPortBlock::LineReset() { m_lines = nullptr; }

void AbstractPortBlock::DrawRect(QPainter *painter) {
  painter->drawEllipse(GetPointLine(), sizeEllipse, sizeEllipse);
}

void AbstractPortBlock::SetLine(LineConnect *Lines) { m_lines = Lines; }

LineConnect *AbstractPortBlock::GetLine() const { return m_lines; }

void AbstractPortBlock::TrackNodes() {
  if (m_lines && m_lines->GetStatus() == connects) {
    m_lines->TrackNodes();
  }
}

OPortBlock::OPortBlock(const QPoint &point, QGraphicsItem *parent)
    : AbstractPortBlock(point, parent) {}

OPortBlock::~OPortBlock() {}

IPortBlock::IPortBlock(const QPoint &point, QGraphicsItem *parent)
    : AbstractPortBlock(point, parent) {}

IPortBlock::~IPortBlock() {}

InPortBlock::InPortBlock(const QPoint &point, IPort *inputPort,
                         QGraphicsItem *parent)
    : IPortBlock(point, parent), m_inputPort(inputPort) {
  QRectF borderRect = boundingRect();
  inPuts = QLine(static_cast<int>(borderRect.left()) + 6,
                 static_cast<int>(borderRect.top() + borderRect.height() / 2),
                 static_cast<int>(borderRect.right()),
                 static_cast<int>(borderRect.top() + borderRect.height() / 2));
}

InPortBlock::~InPortBlock() {}

QPointF InPortBlock::GetPointLine() {
  return static_cast<QPointF>(inPuts.p1());
}

QPainterPath InPortBlock::shape() const {
  QPainterPath path;
  QRectF borderRect = boundingRect();
  path.addRect(borderRect.x() + 7, borderRect.y(), 17, 12);
  return path;
}

IPort *InPortBlock::GetInputPort() { return m_inputPort; }

void InPortBlock::LineReset() {
  AbstractPortBlock::LineReset();
  m_inputPort->Disconnect();
}

void InPortBlock::TryConnect() {
  if (GetLine())
    return;

  OPortBlock *oPort = dynamic_cast<OPortBlock *>(
      scene()->itemAt(mapToScene(GetPointLine()), QTransform()));
  if (oPort) {
    if (!oPort->GetLine()) {
      ConnectPorts(oPort, this);
    }
  }
}

void InPortBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                        QWidget *) {
  DrawRect(painter);
}

void InPortBlock::mousePressEvent(QGraphicsSceneMouseEvent *) {
  if (!GetLine())
    SetLine(new LineConnect(this));
  else {
    if (GetLine()->flags().testFlag(ItemIsMovable)) {
      GetLine()->SetStatus(outToIn);
      GetLine()->grabMouse();
      LineReset();
    }
  }
}

void InPortBlock::DrawRect(QPainter *painter) {
  painter->drawLine(inPuts);
  AbstractPortBlock::DrawRect(painter);
}

OutPortBlock::OutPortBlock(const QPoint &point, OPort *outputPort,
                           QGraphicsItem *parent)
    : OPortBlock(point, parent), m_outputPort(outputPort) {
  QRectF borderRect = boundingRect();
  outPuts = QLine(static_cast<int>(borderRect.left()),
                  static_cast<int>(borderRect.top() + borderRect.height() / 2),
                  static_cast<int>(borderRect.right()) - 6,
                  static_cast<int>(borderRect.top() + borderRect.height() / 2));
}

OutPortBlock::~OutPortBlock() {}

QPointF OutPortBlock::GetPointLine() {
  return static_cast<QPointF>(outPuts.p2());
}

QPainterPath OutPortBlock::shape() const {
  QPainterPath path;
  QRectF borderRect = boundingRect();
  path.addRect(borderRect.x(), borderRect.y(), 17, 12);
  return path;
}

OPort *OutPortBlock::GetOutputPort() { return m_outputPort; }

void OutPortBlock::LineReset() {
  AbstractPortBlock::LineReset();
  m_outputPort->Disconnect();
}

void OutPortBlock::TryConnect() {
  if (GetLine())
    return;

  IPortBlock *iPort = dynamic_cast<IPortBlock *>(
      scene()->itemAt(mapToScene(GetPointLine()), QTransform()));
  if (iPort) {
    if (!iPort->GetLine()) {
      ConnectPorts(this, iPort);
    }
  }
}

void OutPortBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                         QWidget *) {
  DrawRect(painter);
}

void OutPortBlock::mousePressEvent(QGraphicsSceneMouseEvent *) {
  if (!GetLine())
    SetLine(new LineConnect(this));
  else {
    if (GetLine()->flags().testFlag(ItemIsMovable)) {
      GetLine()->SetStatus(inToOut);
      GetLine()->grabMouse();
      LineReset();
    }
  }
}

void OutPortBlock::DrawRect(QPainter *painter) {
  painter->drawLine(outPuts);
  AbstractPortBlock::DrawRect(painter);
}
