#ifndef TOTALDIFFERENCEBLOCK_H
#define TOTALDIFFERENCEBLOCK_H

#include "bilderblocks.h"
#include "ioblock.h"
#include "totaldifferencemodule.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class TotalTifferenceBlock : public IOBlock {

public:
  TotalTifferenceBlock();
  ~TotalTifferenceBlock() override = default;

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
  void InitBlock(QWidget *wparent) override;
  void CreateBlockPorts();

  std::unique_ptr<TotalTifferenceModule> m_module;
};

#endif // TOTALDIFFERENCEBLOCK_H
