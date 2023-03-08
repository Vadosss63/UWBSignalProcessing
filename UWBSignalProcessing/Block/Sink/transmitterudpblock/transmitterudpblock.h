#ifndef TRANSMITTERUDPBLOCK_H
#define TRANSMITTERUDPBLOCK_H

#include "transmitterudpmodule.h"
#include "sinkblock.h"
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


class TransmitterUDPBlock : public SinkBlock
{

public:
    TransmitterUDPBlock();
    ~TransmitterUDPBlock() override = default;

    AbstractBlock *Clone() const override;
    AbstractModule* GetModule() const override;
    QString GetType() const override;
    void RegistrOperationManager(AbstractOperationManager* operationManager) override;

protected:
    // событие отрисовки
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;
    // Создание рисунка
    void DrawImage(QPainter *painter) override;

private:

    void InitBlock(QWidget * ) override;
    void CreateBlockPorts();

    std::unique_ptr<TransmitterUDPModule> m_module;
};

#endif // TRANSMITTERUDPBLOCK_H
