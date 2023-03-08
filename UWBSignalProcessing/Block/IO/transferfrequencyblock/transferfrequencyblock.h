#ifndef TRANSFERFREQUENCYBLOCK_H
#define TRANSFERFREQUENCYBLOCK_H

#include "transferfrequencymodule.h"
#include "ioblock.h"
#include "transferfrequencydialogbox.h"
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

class TransferFrequencyBlock :  public IOBlock
{

public:

    TransferFrequencyBlock();
    ~TransferFrequencyBlock() override = default;

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

    void InitBlock(QWidget*) override;
    void CreateBlockPorts();

    std::unique_ptr<TransferFrequencyModule> m_module = nullptr;
    std::unique_ptr<TransferFrequencyDialogBox> m_dialog = nullptr;

};

#endif // TRANSFERFREQUENCYBLOCK_H
