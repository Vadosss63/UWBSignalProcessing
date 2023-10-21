#include "arithmeticblock.h"

ArithmeticBlock::ArithmeticBlock() : IOBlock("Arithmetic") {
  setToolTip(
      "Выполняет выбор арефметического блока /nиз предложенного списка.");
}

AbstractBlock *ArithmeticBlock::Clone() const { return new ArithmeticBlock(); }

AbstractModule *ArithmeticBlock::GetModule() const { return m_module.get(); }

QString ArithmeticBlock::GetType() const { return "ArithmeticBlock"; }

void ArithmeticBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetMathValue(m_dialog->GetState());
    m_drawText = m_dialog->GetTitleState();
    update();
  }
}

void ArithmeticBlock::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ArithmeticBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();

  painter->setFont(QFont("Fantasy", 18));
  painter->drawText(rect, Qt::AlignCenter, m_drawText);
  painter->setFont(QFont());
}

void ArithmeticBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<ArithmeticModule>();
  m_dialog = std::make_unique<ArithmeticDialogBox>();
  m_drawText = m_dialog->GetTitleState();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void ArithmeticBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new ArithmeticBlock; }

void ArithmeticBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
