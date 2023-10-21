#include "testfilterblock.h"

TestFilterBlock::TestFilterBlock() : IOBlock("Filter") {
  setToolTip("Выполняет фильтрацию входного сигнала");
}

AbstractBlock *TestFilterBlock::Clone() const { return new TestFilterBlock(); }

AbstractModule *TestFilterBlock::GetModule() const { return m_module.get(); }

QString TestFilterBlock::GetType() const { return "TestFilterBlock"; }

void TestFilterBlock::Change() {
  if (m_module && m_dialog) {
    m_module->SetFilterParameters(
        m_dialog->GetSampleFreq(), m_dialog->GetCutFreq1(),
        m_dialog->GetCutFreq2(), m_dialog->GetOrder(),
        static_cast<TestFilterModule::FilterType>(m_dialog->GetFilterType()));

    if (m_dialog->GetPortState()) {
      m_module->SetInputPortsNumber(m_dialog->GetInputPortsNumber());
      m_module->SetOutputPortsNumber(m_dialog->GetOutputPortsNumber());
      ChangeCountPorts();
    }
  }
}

void TestFilterBlock::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void TestFilterBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 12));
  painter->drawText(rect, Qt::AlignCenter, "FILTER");
  painter->setFont(QFont());
}

void TestFilterBlock::ChangeCountPorts() { CreateBlockPorts(); }

void TestFilterBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<TestFilterModule>();
  m_dialog = std::make_unique<FilterDialogBox>();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void TestFilterBlock::CreateBlockPorts() {
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

AbstractBlock *PluginBlock::LoudBlock() const { return new TestFilterBlock; }

void TestFilterBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
