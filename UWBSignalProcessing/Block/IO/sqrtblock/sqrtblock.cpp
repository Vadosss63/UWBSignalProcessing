#include "sqrtblock.h"

SqrtBlock::SqrtBlock() : IOBlock("SQRT"), m_module(nullptr) {
  setToolTip("Вычисляет модуль комплексного числа");
}

AbstractBlock *SqrtBlock::Clone() const { return new SqrtBlock(); }

AbstractModule *SqrtBlock::GetModule() const { return m_module.get(); }

QString SqrtBlock::GetType() const { return "SqrtBlock"; }

void SqrtBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetIsSqrt(m_dialog->GetIsSqrt());
  }
}

void SqrtBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void SqrtBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 10));
  painter->drawText(rect, Qt::AlignCenter, "Огибающая\nсигнала");
  painter->setFont(QFont());
}

void SqrtBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<SqrtModule>();
  m_dialog = std::make_unique<SqrtDialogBox>();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void SqrtBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new SqrtBlock; }

void SqrtBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
