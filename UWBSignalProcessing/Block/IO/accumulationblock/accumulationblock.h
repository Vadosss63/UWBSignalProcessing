#ifndef ACCUMULATIONBLOCK_H
#define ACCUMULATIONBLOCK_H

#include "ioblock.h"
#include "accumulationmodule.h"
#include "accumulationdialogbox.h"
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


class AccumulationBlock : public IOBlock
{

public:
    AccumulationBlock();
    ~AccumulationBlock() override = default;
    AbstractBlock* Clone() const override;
    AbstractModule* GetModule() const override;

    QString GetType() const override;
    void Change() override;
    void RegistrOperationManager(AbstractOperationManager* operationManager) override;

protected:    
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;
    void DrawImage(QPainter *painter) override;

private:
    void InitBlock(QWidget *) override;
    void CreateBlockPorts();

    QVector<QLine> m_image;
    std::unique_ptr<AccumulationModule> m_module = nullptr;
    std::unique_ptr<AccumulationDialogBox> m_dialog = nullptr;
};

#endif // ACCUMULATIONBLOCK_H
