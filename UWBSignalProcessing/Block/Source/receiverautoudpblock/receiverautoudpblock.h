#ifndef RECEIVERAUTOUDPBLOCK_H
#define RECEIVERAUTOUDPBLOCK_H

#include "bilderblocks.h"
#include "receiverautoudpmodule.h"
#include "sourceblock.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class ReceiverAutoUDPBlock : public SourceBlock {

public:
  ReceiverAutoUDPBlock();
  ~ReceiverAutoUDPBlock() override = default;

  AbstractBlock *Clone() const override;
  AbstractModule *GetModule() const override;
  QString GetType() const override;
  void
  RegistrOperationManager(AbstractOperationManager *operationManager) override;

protected:
  // событие отрисовки
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *) override;
  // Создание рисунка
  void DrawImage(QPainter *painter) override;

private:
  void InitBlock(QWidget *) override;
  void CreateBlockPorts();

  std::unique_ptr<ReceiverAutoUDPModule> m_module = nullptr;
};

#endif // RECEIVERAUTOUDPBLOCK_H
