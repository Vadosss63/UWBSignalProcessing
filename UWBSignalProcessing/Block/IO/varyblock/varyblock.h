#ifndef VARYBLOCK_H
#define VARYBLOCK_H

#include "varymodule.h"
#include "varydialogbox.h"
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

class VARYBlock : public IOBlock
{

public:

    VARYBlock();
    ~VARYBlock() override = default;

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

    std::unique_ptr<VARYModule> m_module = nullptr;
    std::unique_ptr<VARYDialogBox> m_dialog = nullptr;

};

#endif // VARYBLOCK_H
