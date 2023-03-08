#ifndef READBLOCK_H
#define READBLOCK_H

#include "readmodule.h"
#include "sourceblock.h"
#include "readdialogbox.h"
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

class ReadBlock: public SourceBlock
{

public:
    ReadBlock();
    ~ReadBlock() override = default;
    AbstractModule* GetModule() const override;
    AbstractBlock* Clone() const override;
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
    std::unique_ptr<ReadModule> m_module = nullptr;
    std::unique_ptr<ReadDialogBox> m_dialog = nullptr;
};

#endif // READBLOCK_H
