#ifndef RECEIVERKAMAZUDPBLOCK_H
#define RECEIVERKAMAZUDPBLOCK_H

#include "sourceblock.h"
#include "receiverkamazudpmodule.h"
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

class ReceiverKamazUDPBlock : public SourceBlock
{

public:

    ReceiverKamazUDPBlock();
    ~ReceiverKamazUDPBlock() override = default;

    AbstractBlock* Clone() const override;
    AbstractModule* GetModule() const override;
    QString GetType() const override;
    void RegistrOperationManager(AbstractOperationManager* operationManager) override;

protected:
    // событие отрисовки
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;
    // Создание рисунка
    void DrawImage(QPainter *painter) override;

private:

    void InitBlock(QWidget *) override;
    void CreateBlockPorts();

    std::unique_ptr<ReceiverKamazUDPModule> m_module = nullptr;
};

#endif // RECEIVERKAMAZUDPBLOCK_H
