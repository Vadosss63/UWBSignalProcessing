#ifndef ACCUMULATIONDUALBLOCK_H
#define ACCUMULATIONDUALBLOCK_H

#include "accumulationdualdialogbox.h"
#include "accumulationdualmodule.h"
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

class AccumulationDualBlock : public IOBlock {
public:
  AccumulationDualBlock();
  ~AccumulationDualBlock() override = default;

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
  QVector<QLine> m_image;
  std::unique_ptr<AccumulationDualModule> m_module = nullptr;
  std::unique_ptr<AccumulationDialogBoxDual> m_dialog = nullptr;
};

#endif // ACCUMULATIONDUALBLOCK_H
