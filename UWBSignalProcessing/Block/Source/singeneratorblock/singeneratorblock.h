#ifndef SINGENERATORBLOCK_H
#define SINGENERATORBLOCK_H

#include "bilderblocks.h"
#include "singeneratordialogbox.h"
#include "singeneratormodule.h"
#include "sourceblock.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  AbstractBlock *LoudBlock() const override;
};

class SinGeneratorBlock : public SourceBlock {

public:
  SinGeneratorBlock();
  ~SinGeneratorBlock() override = default;

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

  std::unique_ptr<SinGeneratorModule> m_module = nullptr;
  std::unique_ptr<SinGeneratorDialogBox> m_dialog = nullptr;
};

#endif // SINGENERATORBLOCK_H
