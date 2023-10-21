#ifndef SQRTBLOCK_H
#define SQRTBLOCK_H

#include "bilderblocks.h"
#include "ioblock.h"
#include "sqrtdialogbox.h"
#include "sqrtmodule.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class SqrtBlock : public IOBlock {

public:
  SqrtBlock();
  ~SqrtBlock() override = default;

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

  std::unique_ptr<SqrtModule> m_module = nullptr;
  std::unique_ptr<SqrtDialogBox> m_dialog = nullptr;
};

#endif // SQRTBLOCK_H
