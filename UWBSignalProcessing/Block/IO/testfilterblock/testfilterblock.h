#ifndef TESTFILTERBLOCK_H
#define TESTFILTERBLOCK_H

#include "bilderblocks.h"
#include "filterview.h"
#include "ioblock.h"
#include "testfilterdialogbox.h"
#include "testfiltermodule.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class TestFilterBlock : public IOBlock {

public:
  TestFilterBlock();
  ~TestFilterBlock() override = default;

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
  void ChangeCountPorts();

private:
  void InitBlock(QWidget *) override;
  void CreateBlockPorts();

  std::unique_ptr<TestFilterModule> m_module = nullptr;
  std::unique_ptr<FilterDialogBox> m_dialog = nullptr;
};

#endif // TESTFILTERBLOCK_H
