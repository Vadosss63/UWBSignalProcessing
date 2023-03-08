#ifndef UPSAMPLINGBLOCK_H
#define UPSAMPLINGBLOCK_H

#include "upsamplingmodule.h"
#include "ioblock.h"
#include "upsamplingdialogbox.h"
#include "bilderblocks.h"

class PluginBlock: public QObject, public BilderBlocks
{
    Q_OBJECT
    Q_INTERFACES(BilderBlocks)
    Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:

    PluginBlock(QObject *parent = nullptr);
    AbstractBlock* LoudBlock() const override;

};

class UpsamplingBlock :  public IOBlock
{

public:

    UpsamplingBlock();
    ~UpsamplingBlock() override = default;

    AbstractBlock* Clone() const override;
    AbstractModule* GetModule() const override;
    QString GetType() const override;
    void Change() override;
    void RegistrOperationManager(AbstractOperationManager* operationManager) override;

protected:
    // событие отрисовки
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;
    // Создание рисунка
    void DrawImage(QPainter *painter) override;

private:

    void InitBlock(QWidget *) override;
    void CreateBlockPorts();

    std::unique_ptr<UpsamplingModule> m_module = nullptr;
    std::unique_ptr<UpsamplingDialogBox> m_dialog = nullptr;

};

#endif // UPSAMPLINGBLOCK_H
