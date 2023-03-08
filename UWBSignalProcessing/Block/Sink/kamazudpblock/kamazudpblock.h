#ifndef KAMAZUDPBLOCK_H
#define KAMAZUDPBLOCK_H


#include "kamazudpmodule.h"
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


class KamazUDPBlock : public SinkBlock
{

public:
    KamazUDPBlock();
    ~KamazUDPBlock() override = default;

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

    std::unique_ptr<KamazUDPModule> m_module;
};

#endif // KAMAZUDPBLOCK_H
