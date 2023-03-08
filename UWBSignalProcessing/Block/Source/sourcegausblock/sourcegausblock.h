#ifndef SOURCEGAUSBLOCK_H
#define SOURCEGAUSBLOCK_H

#include "sourceblock.h"
#include "sourcegausdialogbox.h"
#include "sourcegausmodule.h"
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

class SourceGausBlock : public SourceBlock
{

public:

    SourceGausBlock();
    ~SourceGausBlock() override = default;

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

    std::unique_ptr<SourceGausModule> m_module = nullptr;
    std::unique_ptr<SourceGausDialogBox> m_dialog = nullptr;
};

#endif // SOURCEGAUSBLOCK_H
