#include "vrliscanblock.h"

VRLIScanBlock::VRLIScanBlock() : IOBlock("scan") {
  setToolTip("Преобразует первичную радиолокационную информацию в вторичную");
}

AbstractBlock *VRLIScanBlock::Clone() const { return new VRLIScanBlock(); }

AbstractModule *VRLIScanBlock::GetModule() const { return m_module.get(); }

QString VRLIScanBlock::GetType() const { return "VRLIScanBlock"; }

void VRLIScanBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetCreateVRLI(m_dialog->GetCreateVRLI());
  }
}

void VRLIScanBlock::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void VRLIScanBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 8));
  painter->drawText(rect, Qt::AlignCenter, "ВРЛИ");
  painter->setFont(QFont());
}

void VRLIScanBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<VRLISacanModule>();
  m_dialog = std::make_unique<VRLIScanDialogBox>();
  CreateBlockPorts();
  // класс для отрисовки поля параметров
  SetAbstractDialogCommand(m_dialog.get());
}

void VRLIScanBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new VRLIScanBlock; }

void VRLIScanBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
