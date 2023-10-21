#include "ioblock.h"

IOBlock::IOBlock(const QString &lable, QGraphicsItem *parent)
    : AbstractBlock(lable, parent) {}

IOBlock::~IOBlock() {}

void IOBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *) {
  DrawRect(painter, option);
}

QVariant IOBlock::itemChange(QGraphicsItem::GraphicsItemChange change,
                             const QVariant &value) {
  if (change == ItemPositionHasChanged) {
    for (auto *port : m_inPort)
      port->TrackNodes();

    for (auto *port : m_outPort)
      port->TrackNodes();
  }
  return QGraphicsItem::itemChange(change, value);
}

void IOBlock::ClearPort() {
  if (m_inPort.size()) {
    qDeleteAll(m_inPort);
    m_inPort.clear();
  }
  if (m_outPort.size()) {
    qDeleteAll(m_outPort);
    m_outPort.clear();
  }
}

void IOBlock::SetSizeRect(uint8_t outputs, uint8_t inputs) {
  if (inputs > 3 || outputs > 3) {
    if (inputs > outputs)
      SetRectHeight(20 * inputs);
    else
      SetRectHeight(20 * outputs);
  }
}

QVector<IPortBlock *> IOBlock::GetInPort() const { return m_inPort; }

QVector<OPortBlock *> IOBlock::GetOutPort() const { return m_outPort; }

void IOBlock::CreatePorts(QVector<IPort *> iports, QVector<OPort *> oports) {
  ClearPort();
  uint8_t inputs = static_cast<uint8_t>(iports.size());
  uint8_t outputs = static_cast<uint8_t>(oports.size());
  SetSizeRect(outputs, inputs);
  QRectF rect(GetborderRect());
  short stepIn = static_cast<short>((rect.height()) / (inputs + 1));
  short stepOut = static_cast<short>((rect.height()) / (outputs + 1));
  CreateInPortBlock(iports, inputs, rect, stepIn);
  CreateOutPortBlock(oports, outputs, rect, stepOut);
  update();
  // Переустановливаем родителей для портов
  SetId(GetId());
}

void IOBlock::CreateInPortBlock(QVector<IPort *> iports, uint8_t inputs,
                                const QRectF &rect, short stepIn) {
  for (unsigned short i = 1; i < inputs + 1; i++) {
    InPortBlock *inPortBlock =
        new InPortBlock(QPoint(rect.left() - 24, rect.top() - 6 + i * stepIn),
                        iports[i - 1], this);
    inPortBlock->SetId(i);
    m_inPort.append(inPortBlock);
  }
}

void IOBlock::CreateOutPortBlock(QVector<OPort *> oports, uint8_t outputs,
                                 const QRectF &rect, short stepOut) {
  for (unsigned short i = 1; i < outputs + 1; i++) {
    OutPortBlock *outPortBlock =
        new OutPortBlock(QPoint(rect.right(), rect.top() - 6 + i * stepOut),
                         oports[i - 1], this);
    outPortBlock->SetId(i);
    m_outPort.append(outPortBlock);
  }
}

QList<LinkStruct> IOBlock::GetLinkStruct() {
  QList<LinkStruct> linkStructs;
  QVector<OPortBlock *> ports = GetOutPort();
  foreach (OPortBlock *outPort, ports) {
    LineConnect *line = outPort->GetLine();
    if (line) {
      LinkStruct linkStruct;
      linkStruct.idOutBlock = line->GetOutPort()->GetParentId();
      linkStruct.idOutPort = line->GetOutPort()->GetId();
      linkStruct.idInPort = line->GetInPort()->GetId();
      linkStruct.idInBlock = line->GetInPort()->GetParentId();
      linkStructs.append(linkStruct);
    }
  }
  return linkStructs;
}

// Перехватываем событие установки родительского ID
void IOBlock::SetId(size_t id) {
  AbstractBlock::SetId(id);

  for (auto *in : m_inPort)
    in->SetParentId(GetId());

  for (auto *out : m_outPort)
    out->SetParentId(GetId());
}

void IOBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  for (auto *in : m_inPort)
    in->TryConnect();

  for (auto *out : m_outPort)
    out->TryConnect();

  AbstractBlock::mouseReleaseEvent(event);
}
