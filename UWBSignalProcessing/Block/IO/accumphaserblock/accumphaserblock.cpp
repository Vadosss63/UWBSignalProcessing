#include "accumphaserblock.h"

AccumPhaserBlock::AccumPhaserBlock() : IOBlock("Накопление") {
  setToolTip("Выполняет накопление сигналов с измененной фазой");
}

AbstractBlock *AccumPhaserBlock::Clone() const {
  return new AccumPhaserBlock();
}

AbstractModule *AccumPhaserBlock::GetModule() const { return m_module.get(); }

QString AccumPhaserBlock::GetType() const { return "AccumPhaserBlock"; }

void AccumPhaserBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetCountAccum(m_dialog->GetCountAccum());
    m_module->SetPhase(m_dialog->GetPhase());
  }
}

void AccumPhaserBlock::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void AccumPhaserBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->drawText(static_cast<int>(rect.left()) + 8,
                    static_cast<int>(rect.top()) + 20, "I");
  painter->drawText(static_cast<int>(rect.left()) + 5,
                    static_cast<int>(rect.bottom()) - 10, "Q");
  painter->drawText(static_cast<int>(rect.right()) - 15,
                    static_cast<int>(rect.top()) + 20, "I");
  painter->drawText(static_cast<int>(rect.right()) - 18,
                    static_cast<int>(rect.bottom()) - 10, "Q");
  painter->setFont(QFont("Times", 15));
  painter->drawText(rect, Qt::AlignCenter, "Accum");
  painter->setFont(QFont());
}

void AccumPhaserBlock::InitBlock(QWidget *) {

  m_module = std::make_unique<AccumPhaserModule>();
  m_dialog = std::make_unique<AccumPhaserDialogBox>();

  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void AccumPhaserBlock::CreateBlockPorts() {
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

void AccumPhaserBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new AccumPhaserBlock; }
