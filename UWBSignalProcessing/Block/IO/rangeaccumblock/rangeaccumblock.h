#ifndef RANGEACCUMBLOCK_H
#define RANGEACCUMBLOCK_H

#include "bilderblocks.h"
#include "ioblock.h"
#include "rangeaccumdialogbox.h"
#include "rangeaccummodule.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class RangeAccumBlock : public IOBlock {

public:
  RangeAccumBlock();
  ~RangeAccumBlock() override = default;

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
  void CreateBlockPorts();
  void InitBlock(QWidget *) override;

  std::unique_ptr<RangeAccumModule> m_module = nullptr;
  std::unique_ptr<RangeAccumDialogBox> m_dialog = nullptr;
};

#endif // RANGEACCUMBLOCK_H
