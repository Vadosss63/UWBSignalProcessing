#ifndef ADCNEWDIALOGBOX_H
#define ADCNEWDIALOGBOX_H

#include <map>
#include <QMessageBox>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include "abstractdialogcommand.h"
#include "adcprotocol.h"

class ImitatorWidget;

class ADCNewDialogBox: public AbstractDialogCommand
{

public:

    ADCNewDialogBox();
    ~ADCNewDialogBox() override;

    ADCNewDialogBox(const ADCNewDialogBox& rhs) = delete;
    ADCNewDialogBox& operator=(const ADCNewDialogBox& rhs) = delete;
    QList<std::pair<QString, QVariant>> GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

    int GetFormatAngel() const;

    SAMPLE_NUM GetSampleNum() const;

    AVER_MODE GetAverMode() const;

    AVER_NUM GetAverNum() const;

    ADC_MODE GetAdcMode() const;

    ADC_SYNC GetAdcSync() const;

    int8_t GetShiftQ() const;

    int GetShiftLevelI() const;

    int GetShiftLevelQ() const;

    int GetX() const;
    int GetY() const;
    int GetV() const;

    bool isImitationMode() const;
    void imitationMode(bool enable);

    bool isOneRun() const;

private:

    void CreateWidget();
    void SetupSampleNum(int row, int column);
    void SetupAverSig(int row, int column);
    void SetupAdcMode(int row, int column);
    void SetupSyncMode(int row, int column);
    void SetupAngelFormat(int row, int column);
    void SetupShiftQ(int row, int column);
    void SetupShiftLevelI(int row, int column);
    void SetupShiftLevelQ(int row, int column);

    //создание сетки
    QGridLayout* m_gridLayout;
    QComboBox* m_syncComboBox;
    QComboBox* m_adcModeComboBox;
    QComboBox* m_sampleNumComboBox;
    QComboBox* m_averSigComboBox;
    QGroupBox* m_averSigGroupBox;
    QComboBox* m_agelFormatComboBox;
    QSpinBox* m_shiftQSpinBox;
    QSpinBox* m_shiftLevelISpinBox;
    QSpinBox* m_shiftLevelQSpinBox;
    ImitatorWidget* m_imitatorWidget;
};

class ImitatorWidget: public QVBoxLayout
{
public:
    ImitatorWidget();
    ~ImitatorWidget();


    int GetX() const;
    int GetY() const;
    int GetV() const;
    bool isImitationMode() const;
    bool isOneRun() const;

    void imitationMode(bool enable);


private:

    void CreateWidget();
    QSpinBox* m_x;
    QSpinBox* m_y;
    QSpinBox* m_v;
    QGroupBox* m_imitatorParametr;
    QCheckBox* m_isOneRun;
};


#endif // ADCNEWDIALOGBOX_H
