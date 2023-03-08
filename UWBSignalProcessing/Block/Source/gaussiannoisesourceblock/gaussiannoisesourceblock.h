#ifndef GAUSSIANNOISESOURCEBLOCK_H
#define GAUSSIANNOISESOURCEBLOCK_H

#include "sourceblock.h"
#include "gaussiannoisesourcemodule.h"
#include "gaussiannoisesourcedialogbox.h"
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

class GaussianNoiseSourceBlock: public SourceBlock
{

public:
    GaussianNoiseSourceBlock();
    ~GaussianNoiseSourceBlock() override = default;

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

    std::unique_ptr<GaussianNoiseSourceModule> m_module = nullptr;
    std::unique_ptr<GaussianNoiseSourceDialogBox> m_dialog = nullptr;
};

#endif // GAUSSIANNOISESOURCEBLOCK_H
