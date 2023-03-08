#ifndef ADCNEWBLOCK_H
#define ADCNEWBLOCK_H

#include <vector>
#include "adcnewmodule.h"
#include "sourceblock.h"
#include "adcnewdialogbox.h"
#include "bilderblocks.h"

class PluginBlock: public QObject, public BilderBlocks
{
    Q_OBJECT
    Q_INTERFACES(BilderBlocks)
    Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
    PluginBlock(QObject *parent = nullptr);
    // Создает и настраевает блок
    AbstractBlock* LoudBlock() const override;
};

class ADCNewBlock: public SourceBlock
{

public:

    ADCNewBlock();
    ~ADCNewBlock() override;

    ADCNewBlock(const ADCNewBlock& rhs) = delete;
    ADCNewBlock& operator=(const ADCNewBlock& rhs) = delete;

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
    void ResetBlock();
    // Функция иницилизации
    void InitBlock(QWidget*) override;
    // Указатель на модуль
    ADCNewModule* m_module = nullptr;
    ADCNewDialogBox* m_dialog = nullptr;

};

#endif // ADCNEWBLOCK_H
