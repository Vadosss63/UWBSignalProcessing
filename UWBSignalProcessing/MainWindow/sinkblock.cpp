#include "sinkblock.h"

SinkBlock::SinkBlock(const QString &lable, QGraphicsItem *parent)
    : AbstractBlock(lable, parent) {}

SinkBlock::~SinkBlock() {}

void SinkBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *) {
  DrawRect(painter, option);
}

QVariant SinkBlock::itemChange(QGraphicsItem::GraphicsItemChange change,
                               const QVariant &value) {
  if (change == ItemPositionHasChanged) {
    for (auto *port : m_port)
      port->TrackNodes();
  }
  return QGraphicsItem::itemChange(change, value);
}

QVector<IPortBlock *> SinkBlock::GetInPort() const { return m_port; }

QVector<OPortBlock *> SinkBlock::GetOutPort() const {
  return QVector<OPortBlock *>();
}

void SinkBlock::CreatePorts(QVector<IPort *> ports) {
  uint8_t inputs = ports.size();
  if (inputs > 3)
    SetRectHeight(20 * inputs);

  QRectF rect(GetborderRect());
  uint8_t stepIn = (rect.height()) / (inputs + 1);
  for (uint8_t i = 1; i < inputs + 1; i++) {
    InPortBlock *inPortBlock =
        new InPortBlock(QPoint(rect.left() - 24, rect.top() - 6 + i * stepIn),
                        ports[i - 1], this);
    inPortBlock->SetId(i);
    m_port.append(inPortBlock);
  }

  // Переустановливаем родителей для портов
  SetId(GetId());
}

void SinkBlock::SetId(size_t id) {
  AbstractBlock::SetId(id);

  for (auto *out : m_port)
    out->SetParentId(GetId());
}

void SinkBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  for (auto *port : m_port)
    port->TryConnect();

  AbstractBlock::mouseReleaseEvent(event);
}
