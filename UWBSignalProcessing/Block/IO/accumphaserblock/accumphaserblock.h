#ifndef ACCUMPHASERBLOCK_H
#define ACCUMPHASERBLOCK_H

#include "accumphaserdialogbox.h"
#include "accumphasermodule.h"
#include "bilderblocks.h"
#include "ioblock.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  // Создает и настраевает блок
  AbstractBlock *LoudBlock() const override;
};

class AccumPhaserBlock : public IOBlock {
public:
  AccumPhaserBlock();
  ~AccumPhaserBlock() override = default;

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
  // Функция иницилизации
  void InitBlock(QWidget *) override;
  void CreateBlockPorts();
  // Указатель на модуль
  std::unique_ptr<AccumPhaserModule> m_module = nullptr;
  std::unique_ptr<AccumPhaserDialogBox> m_dialog = nullptr;
};

#endif // ACCUMPHASERBLOCK_H
