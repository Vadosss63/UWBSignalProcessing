#ifndef MATHBLOCK_H
#define MATHBLOCK_H

#include "mathmodule.h"
#include "ioblock.h"
#include "mathdialogbox.h"
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

class MathBlock :  public IOBlock
{

public:

    MathBlock();
    ~MathBlock() override = default;

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

    void CreateBlockPorts();
    void InitBlock(QWidget *) override;

    QString m_drawText = "Math";
    std::unique_ptr<MathModule> m_module = nullptr;
    std::unique_ptr<MathDialogBox> m_dialog = nullptr;

};

#endif // MATHBLOCK_H
