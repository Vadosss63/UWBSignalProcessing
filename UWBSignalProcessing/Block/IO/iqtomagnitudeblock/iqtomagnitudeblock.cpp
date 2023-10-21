#include "iqtomagnitudeblock.h"

IQtoMagnitudeBlock::IQtoMagnitudeBlock() : IOBlock("IQtoMA") {
  setToolTip("Делает преобразования I и Q канала\n к фазе и спектру сигнала");
}

AbstractBlock *IQtoMagnitudeBlock::Clone() const {
  return new IQtoMagnitudeBlock();
}

AbstractModule *IQtoMagnitudeBlock::GetModule() const { return m_module.get(); }

QString IQtoMagnitudeBlock::GetType() const { return "IQtoMagnitudeBlock"; }

void IQtoMagnitudeBlock::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *option,
                               QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void IQtoMagnitudeBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();

  painter->drawText(static_cast<int>(rect.left()) + 8,
                    static_cast<int>(rect.top()) + 20, "I");
  painter->drawText(static_cast<int>(rect.left()) + 5,
                    static_cast<int>(rect.bottom()) - 10, "Q");
  painter->drawText(static_cast<int>(rect.right()) - 18,
                    static_cast<int>(rect.top()) + 20, "A");
  painter->drawText(static_cast<int>(rect.right()) - 25,
                    static_cast<int>(rect.bottom()) - 10, "|M|");

  painter->setFont(QFont("Times", 12));
  painter->drawText(rect, Qt::AlignCenter, "cfft");
  painter->setFont(QFont());
}

void IQtoMagnitudeBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<IQtoMagnitude_AngleModule>();
  CreateBlockPorts();
  SetAbstractDialogCommand(nullptr);
}

void IQtoMagnitudeBlock::CreateBlockPorts() {
  uint8_t sizeIn = m_module->CountInputPort();
  QVector<IPort *> inPorts;
  for (uint8_t i = 0; i < sizeIn; ++i)
    inPorts.push_back(m_module->GetInput(i));

  uint8_t sizeOut = m_module->CountOutputPort();
  QVector<OPort *> outPorts;
  for (uint8_t i = 0; i < sizeOut; ++i)
    outPorts.push_back(m_module->GetOutput(i));

  CreatePorts(inPorts, outPorts);
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new IQtoMagnitudeBlock; }

void IQtoMagnitudeBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
