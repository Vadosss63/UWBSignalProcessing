#ifndef SYNTHETICAPERTUREBLOCK_H
#define SYNTHETICAPERTUREBLOCK_H

#include "syntheticaperturemodule.h"
#include "ioblock.h"
#include "syntheticaperturedialogbox.h"
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

class SyntheticApertureBlock : public IOBlock
{

public:

    SyntheticApertureBlock();
    ~SyntheticApertureBlock() override = default;

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

    std::unique_ptr<SyntheticApertureModule> m_module = nullptr;
    std::unique_ptr<SyntheticApertureDialogBox> m_dialog = nullptr;

};

#endif // SYNTHETICAPERTUREBLOCK_H
