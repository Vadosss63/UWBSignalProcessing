#ifndef WRITETARGETBLOCK_H
#define WRITETARGETBLOCK_H

#include "bilderblocks.h"
#include "sinkblock.h"
#include "writetargetdialogbox.h"
#include "writetargetmodule.h"

class PluginBlock: public QObject, public BilderBlocks
{
    Q_OBJECT
    Q_INTERFACES(BilderBlocks)
    Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:

    PluginBlock(QObject *parent = nullptr);
    AbstractBlock* LoudBlock() const override;

};

class WriteTargetBlock: public SinkBlock
{

public:

    WriteTargetBlock();
    ~WriteTargetBlock() override = default;

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

    std::unique_ptr<WriteTargetModule> m_module = nullptr;
    std::unique_ptr<WriteTargetDialogBox> m_dialog = nullptr;

};

#endif // WRITETARGETBLOCK_H
