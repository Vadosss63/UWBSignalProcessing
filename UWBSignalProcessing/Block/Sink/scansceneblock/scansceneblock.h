#ifndef SCANSCENEBLOCK_H
#define SCANSCENEBLOCK_H

#include "bilderblocks.h"
#include "scanscenedialogbox.h"
#include "scanscenemodule.h"
#include "sinkblock.h"
#include "vrliviewmodule.h"
#include <QRectF>

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  AbstractBlock *LoudBlock() const override;
};

class AbstractScanBlock : public SinkBlock {

public:
  AbstractScanBlock(const QString &title);
  ~AbstractScanBlock() override = default;

protected:
  // событие отрисовки
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *) override;
  // Создание рисунка
  void DrawImage(QPainter *painter) override;

  QConicalGradient GradientImage() const;

private:
  QConicalGradient m_gradientImage;
};

// Класс осуществяющий взаимодействие графического блока и ScanBufferSceneModule
// Содержит в себе мотоды отрисовка графического блока
class ScanSceneBlock : public AbstractScanBlock {

public:
  ScanSceneBlock();
  ~ScanSceneBlock() override = default;

  AbstractBlock *Clone() const override;
  AbstractModule *GetModule() const override;
  QString GetType() const override;
  void Change() override;
  void
  RegistrOperationManager(AbstractOperationManager *operationManager) override;

protected:
  void InitBlock(QWidget *wparent) override;
  void DrawImage(QPainter *painter) override;

private:
  void CreateBlockPorts();

  std::unique_ptr<ScanBufferSceneModule> m_module = nullptr;
  std::unique_ptr<VRLIViewModule> m_moduleVrli = nullptr;
  std::unique_ptr<ScanSceneDialogBox> m_dialog = nullptr;
};

#endif // SCANSCENEBLOCK_H
