#ifndef ADCSERVERBLOCK_H
#define ADCSERVERBLOCK_H

#include "adcdialogbox.h"
#include "adcmodule.h"
#include "bilderblocks.h"
#include "sourceblock.h"
#include <vector>

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class ADCServerBlock : public SourceBlock {

public:
  ADCServerBlock();
  ~ADCServerBlock() override = default;

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
  void AddInitADCiP(uint8_t name);
  void CleanListNameIP();

private:
  void CreateBlockPorts();

  void InitBlock(QWidget *wparent) override;

  bool isInit = false;
  std::vector<uint8_t> m_listNameIP;
  std::unique_ptr<ADCModule> m_module = nullptr;
  std::unique_ptr<ADCServerDialogBox> m_dialog = nullptr;
};

#endif // ADCSERVERBLOCK_H
