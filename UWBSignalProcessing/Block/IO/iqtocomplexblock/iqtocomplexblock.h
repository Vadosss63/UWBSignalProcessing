#ifndef IQTOCOMPLEXBLOCK_H
#define IQTOCOMPLEXBLOCK_H

#include "iqtocomplexmodule.h"
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

class IQToComplexBlock : public IOBlock
{

public:

    IQToComplexBlock();
    ~IQToComplexBlock() override = default;

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
    // выполняет отрисовку портов
    void ChangeCountPorts();

private:

    void InitBlock(QWidget*) override;
    void CreateBlockPorts();

    std::unique_ptr<IQToComplexModule> m_module = nullptr;

};

#endif // IQTOCOMPLEXBLOCK_H
