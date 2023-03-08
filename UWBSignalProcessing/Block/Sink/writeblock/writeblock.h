#ifndef WRITEBLOCK_H
#define WRITEBLOCK_H

#include "bilderblocks.h"
#include "sinkblock.h"
#include "writedialogbox.h"
#include "writemodule.h"

class PluginBlock: public QObject, public BilderBlocks
{
    Q_OBJECT
    Q_INTERFACES(BilderBlocks)
    Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:

    PluginBlock(QObject *parent = nullptr);
    AbstractBlock* LoudBlock() const override;

};

class WriteBlock: public SinkBlock
{

public:

    WriteBlock();
    ~WriteBlock() override = default;

    AbstractBlock *Clone() const override;
    // доступ к абстракт модулю
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


    std::unique_ptr<WriteModule> m_module = nullptr;
    std::unique_ptr<WriteDialogBox> m_dialog = nullptr;

};

#endif // WRITEBLOCK_H
