#ifndef RS485DIALOGBOX_H
#define RS485DIALOGBOX_H

#include <QDialog>
#include <QMap>
#include <QTimer>
#include <string>
#include "ptreecommandswriter.h"

// Класс осуществяющий взаимодействие между пользователем и RecieverControlModule
// Содержит в себе графическое диологовое окно, предоставляющее пользователю возможность
// изменять парамерты RecieverControlModule

namespace Ui {
class RS485DialogBox;
}

class ReciverUWBDialog: public QDialog
{
    Q_OBJECT

    struct Status
    {
        uint8_t dataX0:2;
        uint8_t statusReciver:2;
        uint8_t dataX1:4;
    };

    struct ValueData
    {
        struct Accum
        {
            // количество накоплений для одного элемента
            uint8_t countAccum : 4;
            // режим с одним или двумя наконакоплениями
            uint8_t isTwoGain:1;
            uint8_t :3;
            // количество передатчиков
            uint8_t countTransmiter;
            // количество приемных антенн
            uint8_t countReciverAntenn;
        };

        struct ControlScan
        {
            // азимут сканирования
            uint32_t azimut:8;
            // ручное сканирование
            uint32_t isHandScan:1;
            uint32_t :23;
        };

        Accum accumSynchronition;
        ControlScan controlScan;

        uint32_t DMSynthesizerTune;
        uint8_t syncronizationTune;
        uint8_t bandWidthTune;
        uint8_t dm_amplifierTune;
        uint8_t USP1AmplifierTune;
        uint8_t USP2AmplifierTune;
        uint8_t dm_amplifierTune_2;
        uint8_t USP1AmplifierTune_2;
        uint8_t USP2AmplifierTune_2;
        uint8_t reciverStatus = 0x55;
        uint8_t reciverStatusSynchronition = 0x02;
    };

public:
    ReciverUWBDialog(QWidget *wparent = nullptr);
    ~ReciverUWBDialog();
    ReciverUWBDialog(const ReciverUWBDialog& rhs) = delete;
    ReciverUWBDialog& operator=(const ReciverUWBDialog& rhs) = delete;

protected slots:
    // формирования команд управления
    void SendCommand();
    /// Слоты для изменения значений на дисплее
    // Значения synt ДМ
    void ChangeDm();

    void ChangeAmpDm(int val);
    void ChangeAmpUsp1(int val);
    void ChangeAmpUsp2(int val);

    void ChangeAmpDm_2(int val);
    void ChangeAmpUsp1_2(int val);
    void ChangeAmpUsp2_2(int val);


    void InitValueData();
    void LoudValueDataToUI();

private slots:

    // команды для приемника
    void WriteCommandSyncronizationTune();
    void WriteCommandBandWidthTune();
    void WriteCommandDMAmplifierTune();
    void WriteCommandUSP1AmplifierTune();
    void WriteCommandUSP2AmplifierTune();
    void WriteCommandDMAmplifierTune_2();
    void WriteCommandUSP1AmplifierTune_2();
    void WriteCommandUSP2AmplifierTune_2();
    void WriteCommandDMSynthesizerTuneFast();
    void WriteCommandReboot();
    void WriteCommandSave();
    void WriteCommandStatusReciver();
    void WriteCommandGetSettingReciver();
    // команды для передатчика
    void WriteCommandStatusSynchronition();
    void WriteCommandAccumSynchronition();
    void WriteCommandControlScanSynchronition();
    void WriteCommandRebootSynchronition();
    void WriteCommandSaveSynchronition();


    void ExecuteCommand();

private:
    void ReadCommandSyncronizationTune();
    void ReadCommandBandWidthTune();
    void ReadCommandDMAmplifierTune();
    void ReadCommandUSP1AmplifierTune();
    void ReadCommandUSP2AmplifierTune();
    void ReadCommandDMAmplifierTune_2();
    void ReadCommandUSP1AmplifierTune_2();
    void ReadCommandUSP2AmplifierTune_2();
    void ReadCommandDMSynthesizerTuneFast();
    void ReadCommandReboot();
    void ReadCommandSave();
    void ReadCommandStatusReciver();
    uint32_t ReadCommandGetSettingReciver();

    void ReadCommandStatusSynchronition();
    void ReadCommandAccumSynchronition();
    void ReadCommandControlScanSynchronition();
    void ReadCommandRebootSynchronition();
    void ReadCommandSaveSynchronition();

    double ChangeAmp(int val);
    // проверка отправки сообщения
    bool CheckNoError();

    // настройка соединений
    void MakeConnections();

    void WriteStatus(const QString& status);
    void WriteStatusSynchronition(const QString& status);

    inline uint8_t GetAddress() const;
    inline uint8_t GetAddressSynchronition() const;

    // выполняет соединение активных слотов
    void MakeConnectionsSlot();

    bool GetAnswer();
    bool ReadAnswer();
    void SetupStatus();
    void SetupStatusSynchronition();

    uint32_t CreateRequestGetSetting(uint32_t data);
    void TimerSenderStop();

    // загрузка формы диоллогового окна
    Ui::RS485DialogBox* m_ui;
    PTreeCommandsWriter* m_ptreeCommandsWriter;

    typedef void(ReciverUWBDialog::* SlotExecute)();
    QMap<QObject*,  SlotExecute> m_slots;
    SlotExecute m_slotExecute{};
    QTimer* m_timerSender;
    // состояния данных
    ValueData m_stateValueData;

};

#endif // RS485DIALOGBOX_H
