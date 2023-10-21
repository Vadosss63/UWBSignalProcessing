#ifndef READCOMPLEXBLOCK_H
#define READCOMPLEXBLOCK_H

#include "bilderblocks.h"
#include "readcomplexdialogbox.h"
#include "readcomplexmodule.h"
#include "sourceblock.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class ReadComplexBlock : public SourceBlock {

public:
  ReadComplexBlock();
  ~ReadComplexBlock() override = default;

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

  bool m_isComplex = false;
  std::unique_ptr<ReadComplexModule> m_module = nullptr;
  std::unique_ptr<ReadComplexDialogBox> m_dialog = nullptr;
};

#endif // READCOMPLEXBLOCK_H
