#include "varyblock.h"

VARYBlock::VARYBlock() : IOBlock("ВАРУ") {
  setToolTip("Временное усиление по заданным отсчетам");
}

AbstractBlock *VARYBlock::Clone() const { return new VARYBlock(); }

AbstractModule *VARYBlock::GetModule() const { return m_module.get(); }

QString VARYBlock::GetType() const { return "VARYBlock"; }

void VARYBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetVARYCoeff(m_dialog->GetVARYCoeff());
  }
}

void VARYBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void VARYBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();

  painter->setFont(QFont("Times", 12));
  painter->drawText(rect, Qt::AlignCenter, "ВАРУ");
  painter->setFont(QFont());
}

void VARYBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<VARYModule>();
  m_dialog = std::make_unique<VARYDialogBox>();
  CreateBlockPorts();
  // класс для отрисовки поля параметров
  SetAbstractDialogCommand(m_dialog.get());
}

void VARYBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new VARYBlock; }

void VARYBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
