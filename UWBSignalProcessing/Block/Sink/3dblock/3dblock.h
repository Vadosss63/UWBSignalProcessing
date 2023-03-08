#ifndef D3BLOCK_H
#define D3BLOCK_H

#include <QRectF>
#include <QLinearGradient>
#include "bilderblocks.h"
#include "3dmodule.h"
#include "sinkblock.h"
#include "3ddialogbox.h"

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

class D3Block : public AbstractScanBlock
{

public:

    D3Block();
    ~D3Block() override = default;

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

    std::unique_ptr<D3Module> m_module = nullptr;
    std::unique_ptr<D3DialogBox> m_dialog = nullptr;
};

#endif // D3BLOCK_H
