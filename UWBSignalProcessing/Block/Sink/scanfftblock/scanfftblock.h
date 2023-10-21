#ifndef SCANFFTBLOCK_H
#define SCANFFTBLOCK_H

#include "bilderblocks.h"
#include "scanfftdialogbox.h"
#include "scanfftmodule.h"
#include "sinkblock.h"
#include <QLinearGradient>
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
  QConicalGradient &GradientImage();

private:
  QConicalGradient m_gradientImage;
};

class ScanfftBlock : public AbstractScanBlock {

public:
  ScanfftBlock();
  ~ScanfftBlock() override = default;

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

  std::unique_ptr<ScanfftModule> m_module = nullptr;
  std::unique_ptr<ScanfftDialogBox> m_dialog = nullptr;
};

#endif // SCANFFTBLOCK_H
