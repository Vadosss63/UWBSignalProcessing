#ifndef MASKFILTERBLOCK_H
#define MASKFILTERBLOCK_H

#include "maskfiltermodule.h"
#include "maskfilterdialogbox.h"
#include "ioblock.h"
#include "bilderblocks.h"


class PluginBlock: public QObject, public BilderBlocks
{
    Q_OBJECT
    Q_INTERFACES(BilderBlocks)
    Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" "maskfilterblock.json")

public:
    PluginBlock(QObject *parent = nullptr);
    AbstractBlock* LoudBlock() const override;
};

class MaskFilterBlock : public IOBlock
{

public:

    MaskFilterBlock();
    ~MaskFilterBlock() override = default;

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

    void InitBlock(QWidget* ) override;
    void CreateBlockPorts();

    std::unique_ptr<MaskFilterModule> m_module = nullptr;
    std::unique_ptr<MaskFilterDialogBox> m_dialog = nullptr;
};

#endif // MASKFILTERBLOCK_H
