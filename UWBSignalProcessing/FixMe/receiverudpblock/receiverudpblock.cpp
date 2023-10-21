#include "receiverudpblock.h"
#include <QTimer>

ReceiverUDPBlock::ReceiverUDPBlock()
    : SourceBlock("Приемник"), m_module(nullptr) {
  setToolTip("Выполняет прием данных по протоколу UDP");
}

ReceiverUDPBlock::~ReceiverUDPBlock() {}

AbstractBlock *ReceiverUDPBlock::Clone() const {
  return new ReceiverUDPBlock();
}

dspm::AbstractModule &ReceiverUDPBlock::GetModule() { return *m_module; }

QString ReceiverUDPBlock::GetType() { return "ReceiverUDPBlock"; }

void ReceiverUDPBlock::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *) {
  DrawRect(painter, option);
  DrawImage(painter);
}

void ReceiverUDPBlock::DrawImage(QPainter *painter) {
  const QRectF &rect = GetborderRect();
  painter->setFont(QFont("Times", 10));
  painter->drawText(rect, Qt::AlignCenter, "UDP");
  painter->setFont(QFont());
}

void ReceiverUDPBlock::InitBlock(QWidget *wparent) {
  dspm::ReceiverUDPModule *module = new dspm::ReceiverUDPModule(this);
  if (m_module)
    delete m_module;
  m_module = module;
  CreatePorts(module);

  // класс для отрисовки поля параметров
  setAbstractDialogCommand(new ReceiverUDPDialogBox(module, wparent));
}

PluginBlock::PluginBlock(QObject *parent) : QObject(parent) {}

AbstractBlock *PluginBlock::LoudBlock() const { return new ReceiverUDPBlock; }
