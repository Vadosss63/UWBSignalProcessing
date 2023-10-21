#include "mathblock.h"

MathBlock::MathBlock() : IOBlock("Math") {
  setToolTip("Выполняет выбор математического блока из предложенного списка.");
}

AbstractBlock *MathBlock::Clone() const { return new MathBlock(); }

AbstractModule *MathBlock::GetModule() const { return m_module.get(); }

QString MathBlock::GetType() const { return "MathBlock"; }

void MathBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetMathValue(m_dialog->GetMathState());
    m_drawText = m_dialog->GetTitleState();
    update();
  }
}

void MathBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void MathBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();

  painter->setFont(QFont("Times", 12));
  painter->drawText(rect, Qt::AlignCenter, m_drawText);
  painter->setFont(QFont());
}

void MathBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<MathModule>();
  m_dialog = std::make_unique<MathDialogBox>();
  m_drawText = m_dialog->GetTitleState();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void MathBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new MathBlock; }

void MathBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
