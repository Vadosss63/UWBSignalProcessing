#include "upsamplingblock.h"

UpsamplingBlock::UpsamplingBlock() : IOBlock("Upsampling") {
  setToolTip("Выполняет повышение частоты дискретизации в N раз");
}

AbstractBlock *UpsamplingBlock::Clone() const { return new UpsamplingBlock(); }

AbstractModule *UpsamplingBlock::GetModule() const { return m_module.get(); }

QString UpsamplingBlock::GetType() const { return "UpsamplingBlock"; }

void UpsamplingBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetUpsamplingValue(m_dialog->GetUpsamplingValue());
  }
}

void UpsamplingBlock::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void UpsamplingBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 12));
  painter->drawText(rect, Qt::AlignCenter, "UpSampling");
  painter->setFont(QFont());
}

void UpsamplingBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<UpsamplingModule>();
  m_dialog = std::make_unique<UpsamplingDialogBox>();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void UpsamplingBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new UpsamplingBlock; }

void UpsamplingBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
