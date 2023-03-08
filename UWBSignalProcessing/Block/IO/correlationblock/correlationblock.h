#ifndef CORRELATIONBLOCK_H
#define CORRELATIONBLOCK_H

#include "correlationmodule.h"
#include "ioblock.h"
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

class CorrelationBlock : public IOBlock
{

public:
    CorrelationBlock();
    ~CorrelationBlock() override = default;

    AbstractBlock* Clone() const override;
    AbstractModule* GetModule() const override;
    QString GetType() const override;
    void RegistrOperationManager(AbstractOperationManager* operationManager) override;

protected:
    // событие отрисовки
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;
    // Создание рисунка
    void DrawImage(QPainter* painter) override;

private:

    void InitBlock(QWidget *) override;
    void CreateBlockPorts();

    std::unique_ptr<Correlationmodule> m_module;
};

#endif // CORRELATIONBLOCK_H
