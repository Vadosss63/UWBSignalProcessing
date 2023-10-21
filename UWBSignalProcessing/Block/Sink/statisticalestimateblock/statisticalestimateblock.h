#ifndef STATISTICALESTIMATEBLOCK_H
#define STATISTICALESTIMATEBLOCK_H

#include "bilderblocks.h"
#include "sinkblock.h"
#include "statisticalestimatedialogbox.h"
#include "statisticalestimatemodule.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class StatisticaleStimateBlock : public SinkBlock {

public:
  StatisticaleStimateBlock();
  ~StatisticaleStimateBlock() override = default;

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

  std::unique_ptr<StatisticaleStimateModule> m_module = nullptr;
  std::unique_ptr<StatisticaleStimateDialogBox> m_dialog = nullptr;
};

#endif // STATISTICALESTIMATEBLOCK_H
