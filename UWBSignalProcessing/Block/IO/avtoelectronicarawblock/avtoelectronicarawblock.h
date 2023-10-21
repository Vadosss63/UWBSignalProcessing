#ifndef AVTOELECTRONICARAWBLOCK_H
#define AVTOELECTRONICARAWBLOCK_H

#include "avtoelectronicarawdialogbox.h"
#include "avtoelectronicarawmodule.h"
#include "bilderblocks.h"
#include "ioblock.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class AvtoelectronicaRawBlock : public IOBlock {

public:
  AvtoelectronicaRawBlock();
  ~AvtoelectronicaRawBlock() override = default;

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

  std::unique_ptr<AvtoelectronicaRawModule> m_module = nullptr;
  std::unique_ptr<AvtoelectronicaRawDialogBox> m_dialog = nullptr;
};

#endif // AVTOELECTRONICARAWBLOCK_H
