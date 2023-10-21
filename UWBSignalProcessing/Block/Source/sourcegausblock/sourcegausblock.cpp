#include "sourcegausblock.h"

SourceGausBlock::SourceGausBlock() : SourceBlock("Gaus") {
  setToolTip("Генератор гаусса");
}

AbstractBlock *SourceGausBlock::Clone() const { return new SourceGausBlock(); }

AbstractModule *SourceGausBlock::GetModule() const { return m_module.get(); }

QString SourceGausBlock::GetType() const { return "SourceGausBlock"; }

void SourceGausBlock::Change() {
  if (m_module && m_dialog) {
    m_module->ChangeParametersGenerator(
        m_dialog->GetAmplitude(), m_dialog->GetLenght(), m_dialog->GetSigma(),
        m_dialog->GetExpectation());
  }
}

void SourceGausBlock::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option, QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void SourceGausBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();

  QPoint center(static_cast<int>(rect.width()) / 2,
                static_cast<int>(rect.height()) / 2);
  painter->drawEllipse(center, static_cast<int>(rect.height() / 2 - 5),
                       static_cast<int>(rect.height() / 2 - 5));
  QPainterPath path;
  path.moveTo(rect.left() + 8, (rect.height()) / 2 + 3);

  path.cubicTo(rect.width() / 2 - 6, rect.top(), rect.width() / 2 + 1,
               (rect.height()) / 2, rect.width() / 2, (rect.height()) / 2);

  path.cubicTo(rect.width() / 2 + 7, rect.bottom(), rect.width() / 2 + 25,
               (rect.height()) / 2, rect.right() - 8, (rect.height()) / 2);

  painter->drawPath(path);
}

void SourceGausBlock::InitBlock(QWidget *) {
  m_module = std::make_unique<SourceGausModule>();
  m_dialog = std::make_unique<SourceGausDialogBox>();
  CreateBlockPorts();
  SetAbstractDialogCommand(m_dialog.get());
}

void SourceGausBlock::CreateBlockPorts() {
  uint8_t size = m_module->CountOutputPort();
  QVector<OPort *> ports;
  for (uint8_t i = 0; i < size; ++i)
    ports.push_back(m_module->GetOutput(i));

  CreatePorts(ports);
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new SourceGausBlock; }

void SourceGausBlock::RegistrOperationManager(
    AbstractOperationManager *operationManager) {
  if (m_module)
    m_module->RegistrOperationManager(operationManager);
}
