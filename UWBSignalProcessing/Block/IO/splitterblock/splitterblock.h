#ifndef SPLITTERBLOCK_H
#define SPLITTERBLOCK_H

#include "bilderblocks.h"
#include "ioblock.h"
#include "splitterdialogbox.h"
#include "splittermodule.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class SplitterBlock : public IOBlock {

public:
  SplitterBlock();
  ~SplitterBlock() override = default;

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
  // выполняет отрисовку портов
  void ChangeCountPorts();

private:
  void InitBlock(QWidget *) override;
  void CreateBlockPorts();

  std::unique_ptr<SplitterModule> m_module = nullptr;
  std::unique_ptr<SplitterDialogBox> m_dialog = nullptr;
};

#endif // SPLITTERBLOCK_H
