#include "complextoiqblock.h"

ComplexToIQBlock::ComplexToIQBlock() : IOBlock("Complex2IQ") {
  setToolTip(
      "Принимает на вход комплексный сигнал, на выходе выдает квадратуры");
}

AbstractBlock *ComplexToIQBlock::Clone() const {
  return new ComplexToIQBlock();
}

AbstractModule *ComplexToIQBlock::GetModule() const { return m_module.get(); }

QString ComplexToIQBlock::GetType() const { return "ComplexToIQBlock"; }

void ComplexToIQBlock::Change() {}

void ComplexToIQBlock::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ComplexToIQBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();

  painter->drawText(static_cast<int>(rect.left()) + 8,
                    static_cast<int>(rect.center().y()) + 5, "C");
  painter->drawText(static_cast<int>(rect.right()) - 18,
                    static_cast<int>(rect.top()) + 20, "I");
  painter->drawText(static_cast<int>(rect.right()) - 18,
                    static_cast<int>(rect.bottom()) - 10, "Q");
  painter->setFont(QFont());
}

void ComplexToIQBlock::ChangeCountPorts() { CreateBlockPorts(); }

void ComplexToIQBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<ComplexToIQModule>();
  CreateBlockPorts();
}

void ComplexToIQBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new ComplexToIQBlock; }

void ComplexToIQBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
