#ifndef ARITHMETICBLOCK_H
#define ARITHMETICBLOCK_H

#include "arithmeticdialogbox.h"
#include "arithmeticmodule.h"
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

class ArithmeticBlock : public IOBlock {

public:
  ArithmeticBlock();
  ~ArithmeticBlock() override = default;

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
  QString m_drawText = "Arith";

  std::unique_ptr<ArithmeticModule> m_module = nullptr;
  std::unique_ptr<ArithmeticDialogBox> m_dialog = nullptr;
};

#endif // ARITHMETICBLOCK_H
