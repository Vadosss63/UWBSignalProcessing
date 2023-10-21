#ifndef COMPLEXTOIQBLOCK_H
#define COMPLEXTOIQBLOCK_H

#include "bilderblocks.h"
#include "complextoiqmodule.h"
#include "ioblock.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class ComplexToIQBlock : public IOBlock {

public:
  ComplexToIQBlock();
  ~ComplexToIQBlock() override = default;

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

  std::unique_ptr<ComplexToIQModule> m_module = nullptr;
};

#endif // COMPLEXTOIQBLOCK_H
