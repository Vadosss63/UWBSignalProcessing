#ifndef GAINBLOCK_H
#define GAINBLOCK_H

#include "bilderblocks.h"
#include "gaindialogbox.h"
#include "gainmodule.h"
#include "ioblock.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  ~PluginBlock() override = default;
  AbstractBlock *LoudBlock() const override;
};

class GainBlock : public IOBlock {
public:
  GainBlock();
  ~GainBlock() override = default;

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

  QVector<QLine> m_image;
  std::unique_ptr<GainModule> m_module = nullptr;
  std::unique_ptr<GainDialogBox> m_dialog = nullptr;
};

#endif // GAINBLOCK_H
