#include "fftblock.h"

FFTBlock::FFTBlock() : IOBlock("REtoMA"), m_module(nullptr) {
  setToolTip("Делает преобразования сигнала\n к фазе и спектру");
}

AbstractBlock *FFTBlock::Clone() const { return new FFTBlock(); }

AbstractModule *FFTBlock::GetModule() const { return m_module.get(); }

QString FFTBlock::GetType() const { return "FFTBlock"; }

void FFTBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void FFTBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();

  painter->drawText(static_cast<int>(rect.left()) + 8,
                    static_cast<int>(rect.center().y()) + 5, "Re");
  painter->drawText(static_cast<int>(rect.right()) - 18,
                    static_cast<int>(rect.top()) + 20, "A");
  painter->drawText(static_cast<int>(rect.right()) - 25,
                    static_cast<int>(rect.bottom()) - 10, "|M|");

  painter->setFont(QFont("Times", 12));
  painter->drawText(rect, Qt::AlignCenter, "fft");
  painter->setFont(QFont());
}

void FFTBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<FFTmodule>();
  CreateBlockPorts();
}

void FFTBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new FFTBlock; }

void FFTBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
