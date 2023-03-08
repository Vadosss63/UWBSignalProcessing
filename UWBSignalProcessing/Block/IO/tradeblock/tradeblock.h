#ifndef TRADEBLOCK_H
#define TRADEBLOCK_H

#include "trademodule.h"
#include "ioblock.h"
#include "tradedialogbox.h"
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

class TradeBlock : public IOBlock
{

public:

    TradeBlock();
    ~TradeBlock() override;

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

    std::unique_ptr<TradeModule> m_module = nullptr;
    std::unique_ptr<TradeDialogBox> m_dialog = nullptr;

};

#endif // TRADEBLOCK_H
