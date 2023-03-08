#ifndef D3TARGETBLOCK_H
#define D3TARGETBLOCK_H

#include <QRectF>
#include <QLinearGradient>
#include "bilderblocks.h"
#include "3dtargetmodule.h"
#include "sinkblock.h"
#include "3dtargetdialogbox.h"

class PluginBlock: public QObject, public BilderBlocks
{
    Q_OBJECT
    Q_INTERFACES(BilderBlocks)
    Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
    PluginBlock(QObject *parent = nullptr);
    AbstractBlock* LoudBlock() const override;

};

class AbstractScanBlock : public SinkBlock
{

public:

    AbstractScanBlock(const QString &title);
    ~AbstractScanBlock() override = default;

protected:
    // событие отрисовки
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;
    // Создание рисунка
    void DrawImage(QPainter *painter) override;
    QConicalGradient& GradientImage();

private:
    QConicalGradient m_gradientImage;
};

class D3TargetBlock : public AbstractScanBlock
{

public:

    D3TargetBlock();
    ~D3TargetBlock() override = default;

    AbstractBlock* Clone() const override;
    AbstractModule* GetModule() const override;
    QString GetType() const override;
    void Change() override;
    void RegistrOperationManager(AbstractOperationManager* operationManager) override;

protected:

    void InitBlock(QWidget*) override;
    void DrawImage(QPainter* painter) override;

private:
    void CreateBlockPorts();

    std::unique_ptr<D3TargetModule> m_module = nullptr;
    std::unique_ptr<D3TargetDialogBox> m_dialog = nullptr;
};

#endif // D3TARGETBLOCK_H
