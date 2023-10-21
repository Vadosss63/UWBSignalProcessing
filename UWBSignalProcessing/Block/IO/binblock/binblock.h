#ifndef BINBLOCK_H
#define BINBLOCK_H

#include "bilderblocks.h"
#include "bindialogbox.h"
#include "binmodule.h"
#include "ioblock.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class BinBlock : public IOBlock {

public:
  BinBlock();
  ~BinBlock() override = default;

  AbstractBlock *Clone() const override;
  AbstractModule *GetModule() const override;
  QString GetType() const override;
  void Change() override;
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

  std::unique_ptr<BinModule> m_module = nullptr;
  std::unique_ptr<BinDialogBox> m_dialog = nullptr;
};

#endif // BINBLOCK_H
