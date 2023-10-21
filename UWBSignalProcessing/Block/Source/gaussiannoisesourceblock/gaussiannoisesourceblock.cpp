#include "gaussiannoisesourceblock.h"

GaussianNoiseSourceBlock::GaussianNoiseSourceBlock() : SourceBlock("Noise") {
  setToolTip("Генератор гауссовского шума");
}

void GaussianNoiseSourceBlock::Change() {
  if (m_module && m_dialog)
    m_module->SetParameters(m_dialog->GetSignalSize(),
                            m_dialog->GetDeviation());
}

AbstractBlock *GaussianNoiseSourceBlock::Clone() const {
  return new GaussianNoiseSourceBlock();
}

AbstractModule *GaussianNoiseSourceBlock::GetModule() const {
  return m_module.get();
}

QString GaussianNoiseSourceBlock::GetType() const {
  return "GaussianNoiseSourceBlock";
}

void GaussianNoiseSourceBlock::paint(QPainter *painter,
                                     const QStyleOptionGraphicsItem *option,
                                     QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void GaussianNoiseSourceBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 10));
  painter->drawText(rect, Qt::AlignCenter, "Gaussian \n Noise");
  painter->setFont(QFont());
}

void GaussianNoiseSourceBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<GaussianNoiseSourceModule>();
  m_dialog = std::make_unique<GaussianNoiseSourceDialogBox>();

  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void GaussianNoiseSourceBlock::CreateBlockPorts() {
  uint8_t size = m_module->CountOutputPort();
  QVector<OPort *> ports;
  for (uint8_t i = 0; i < size; ++i) {
    ports.push_back(m_module->GetOutput(i));
  }
  CreatePorts(ports);
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const {
  return new GaussianNoiseSourceBlock;
}

void GaussianNoiseSourceBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
