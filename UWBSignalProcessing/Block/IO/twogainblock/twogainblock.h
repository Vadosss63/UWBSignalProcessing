#ifndef TWOGAINBLOCK_H
#define TWOGAINBLOCK_H

#include "twogainmodule.h"
#include "ioblock.h"
#include "twogaindialogbox.h"
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

class TwoGainBlock : public IOBlock
{
public:

    TwoGainBlock();
    ~TwoGainBlock() override = default;

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

    QVector<QLine> m_image;
    std::unique_ptr<TwoGainModule> m_module = nullptr;
    std::unique_ptr<TwoGainDialogBox> m_dialog = nullptr;

};

#endif // TWOGAINBLOCK_H
