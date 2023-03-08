#ifndef EXPBLOCK_H
#define EXPBLOCK_H

#include "expmodule.h"
#include "ioblock.h"
#include "expdialogbox.h"
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

class ExpBlock : public IOBlock
{

public:
    ExpBlock();
    ~ExpBlock() override = default;

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

    std::unique_ptr<ExpModule> m_module = nullptr;
    std::unique_ptr<ExpDialogBox> m_dialog = nullptr;
};

#endif // EXPBLOCK_H
