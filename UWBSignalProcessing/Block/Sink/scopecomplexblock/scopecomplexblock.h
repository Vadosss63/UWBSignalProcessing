#ifndef SCOPECOMPLEXBLOCK_H
#define SCOPECOMPLEXBLOCK_H

#include "bilderblocks.h"
#include "scopecomplexdialogbox.h"
#include "scopecomplexmodule.h"
#include "sinkblock.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class ScopeComplexBlock : public SinkBlock {

public:
  ScopeComplexBlock();
  ~ScopeComplexBlock() override = default;

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
  void InitBlock(QWidget *wparent) override;
  void CreateBlockPorts();

  std::unique_ptr<ScopeComplexModule> m_module = nullptr;
  std::unique_ptr<ScopeComplexDialogBox> m_dialog = nullptr;
};

#endif // SCOPECOMPLEXBLOCK_H
