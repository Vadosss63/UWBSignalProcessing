#ifndef ADCSERVERDIALOGBOX_H
#define ADCSERVERDIALOGBOX_H

#include "abstractdialogcommand.h"
#include "adcmodule.h"
#include <QHostAddress>
#include <QMessageBox>
#include <map>

namespace Ui {
class UDPServerDialogBox;
}

// Регистр адреса и данных АЦП формирования слова управления
struct RegisterControlBoardsADC_ADDR {
  enum class ADDR : uint8_t { ADDRESS_ADDR = 0x0E, ADDRESS_DATA = 0x0F };
  uint8_t SPI_ADDR;
  uint8_t SPI_DATA_ADC;
};

// Регистр управления
struct ADC_ALL_RegisterControl {
  enum class ADDR : uint8_t { ADDRESS = 0x80 };
  uint8_t TX_EN : 1;
  uint8_t EXT_CLK_EN : 1;
  uint8_t NS1_EN : 1;
  uint8_t DATA_X : 5;
};

// Регистр режима работы
struct ADC_ALL_RegisterWorkMode {
  enum class ADDR : uint8_t { ADDRESS = 0x81 };
  uint8_t WORK_MODE : 2;
  uint8_t ADC_MODE : 2;
  uint8_t DAC_MODE : 2;
  uint8_t SDRAM_MODE : 1;
  uint8_t DATA_X : 1;
};

// Регистр длины излучаемого сигнала
struct ADC_ALL_RegisterCOUNT_TX {
  enum class ADDR : uint8_t { ADDRESS = 0x82 };
  uint8_t TX_COUNT;
};

// Регистр количества вводимых отсчетов АЦП
struct ADC_ALL_RegisterCountRX {
  enum class ADDR : uint8_t { ADDRESS = 0x83 };
  uint8_t CountRX;
};

// Регистр задержки ввода отсчетов АЦП
struct ADC_ALL_RegisterRX_DELAY {
  enum class ADDR : uint8_t { ADDRESS = 0x84 };
  uint8_t RX_DELAY;
};

// Регистр задержки ввода отсчетов АЦП
struct ADC_ALL_ACCUM {
  enum class ADDR : uint8_t { ADDRESS = 0x85 };
  uint8_t RX_MODE : 4;
  uint8_t DATA_X : 4;
};

////////////////////////////////////////

// Регистр управления
struct RegisterControl {
  enum class ADDR : uint8_t { ADDRESS = 0x00 };
  uint8_t adress = static_cast<uint8_t>(ADDR::ADDRESS);
  uint8_t TX_EN : 1;
  uint8_t OUTS_EN : 1;
  uint8_t G1_EN : 1;
  uint8_t G1_ADDR_SCLR : 1;
  uint8_t TEST_DATA : 1;
  uint8_t TEST_BEAR : 1;
  uint8_t DATA_X : 2;
};

// Регистр управления LVDS
struct RegisterControlLVDS {
  enum class ADDR : uint8_t {
    ADDRESS_LVDS0 = 0x01,
    ADDRESS_LVDS1 = 0x02,
    ADDRESS_LVDS2 = 0x03,
    ADDRESS_LVDS3 = 0x04,
  };
  uint8_t adress;
  uint8_t DEFAULT : 1;
  uint8_t INV_EN : 1;
  uint8_t RISE : 3;
  uint8_t FALL : 3;
};

// Регистр управления LVTTL
struct RegisterControlLVTTL {
  enum class ADDR : uint8_t { ADDRESS = 0x05 };
  uint8_t adress = static_cast<uint8_t>(ADDR::ADDRESS);
  uint8_t TRX;
};

// Регистр управления TTL
struct RegisterControlTTL {
  enum class ADDR : uint8_t { ADDRESS = 0x06 };
  uint8_t adress = static_cast<uint8_t>(ADDR::ADDRESS);
  uint8_t PWROUT0 : 1;
  uint8_t PWROUT1 : 1;
  uint8_t DATA_X : 6;
};

// Регистр длинны излучаемого сигнала
struct RegisterLengthTX {
  enum class ADDR : uint8_t { ADDRESS = 0x07 };
  uint8_t adress = static_cast<uint8_t>(ADDR::ADDRESS);
  uint8_t LengthTX;
};

// Регистр периода излучаемого сигнала 2 байта байт
struct RegisterTXPERIOD {
  enum class ADDR : uint8_t {
    ADDRESS_L = 0x08,
    ADDRESS_H = 0x09,
  };
  uint8_t adress;
  uint8_t TX_PERIOD;
};

// Регистр тактовых сигналов 4 байта
struct RegisterCLOCKS_TUNE {
  enum class ADDR : uint8_t {
    ADDRESS_TUNE0 = 0x0A,
    ADDRESS_TUNE1 = 0x0B,
    ADDRESS_TUNE2 = 0x0C,
    ADDRESS_TUNE3 = 0x0D,
  };
  uint8_t adress;
  uint8_t CLOCKS_TUNE;
};

// Регистр данных буфера памяти излучаемого сигнала АЦП
struct RegisterControlBoardsADC_BUFFER_MEMARY {
  enum class ADDR : uint8_t { ADDRESS = 0x10 };
  uint8_t adress = static_cast<uint8_t>(ADDR::ADDRESS);
  uint8_t G1_DATA : 5;
  uint8_t DATA_X : 3;
};

// Регистр дополнительных функций
struct RegisterAF {
  enum class ADDR : uint8_t { ADDRESS = 0x11 };
  RegisterAF() {
    LED0 = 0;
    LED1 = 0;
    RX_MODE = 10;
  }
  uint8_t adress = static_cast<uint8_t>(ADDR::ADDRESS);
  uint8_t BRD_COUNT : 2;
  uint8_t LED0 : 1;
  uint8_t LED1 : 1;
  uint8_t RX_MODE : 4;
};

struct StatusADC {
  StatusADC() { Reset(); }
  void Reset() {
    for (int i = 0; i < 16; ++i) {
      data_X[i] = 0;
    }
    for (int i = 0; i < 14; ++i) {
      data_X0[i] = 0;
    }
    for (int i = 0; i < 4; ++i) {
      controlLVDS[i] = 0;
      clocksTune[i] = 0;
    }
    for (int i = 0; i < 2; ++i) {
      txPeriod[i] = 0;
      controlBoardsADC_ADDR[i] = 0;
    }
  }
  uint8_t data_X[16];
  uint8_t control = 0;
  uint8_t controlLVDS[4];
  uint8_t controlLVTTL = 0;
  uint8_t controlTTL = 0;
  uint8_t lengthTX = 0;
  uint8_t txPeriod[2];
  uint8_t clocksTune[4];
  uint8_t controlBoardsADC_ADDR[2];
  uint8_t controlBoardsADC_bufferMemary = 0;
  uint8_t registerAF = 0;
  uint8_t data_X0[14];
};

class ADCServerDialogBox : public AbstractDialogCommand {
  Q_OBJECT
  struct OldValues {
    RegisterTXPERIOD signalPeriod_H;           // period = 650
    RegisterTXPERIOD signalPeriod_L;           //= 0
    ADC_ALL_RegisterRX_DELAY adcDelay;         // adc delay = 0
    RegisterLengthTX signalLength;             // signal length = 255
    ADC_ALL_RegisterWorkMode ufosAllRegimeReg; // workMode, continuos/strob = 0
    ADC_ALL_RegisterControl ufosAllControlReg; // on/off, clk, 1ns = 0x0
    RegisterControl adcControlReg; // on/off, LVDS, azimuth, 1GHz, TestData
    ADC_ALL_RegisterCountRX ufosAllADCNumberPosition; // adcNumber
    RegisterAF registerAF;                            // count Boards ADC
  };

public:
  ADCServerDialogBox(ADCModule *parent, QWidget *wparent = nullptr);
  ~ADCServerDialogBox() override;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

public slots:
  void CheckIPs();

signals:
  // сигнал о добавлении в список нового IP
  void InitADCiP(uint8_t name);
  // сигнал сброса листа с IP
  void ResetIPNameList();

protected slots:
  void SendCommandAbstractModule();
  void ShowMassageResetADC();

private slots:
  void ChangedSignalLength(int value);
  void ChangedADCDelay(int value);
  void ChangedSignalFreaq(double value);
  void ChangedWorkMode(bool checked);

  void ShowDialogMassage(const QString &index);

  void SwitchADCOn();
  void SwitchADCOff();

  void SetupDelayADC();

private:
  void InitOldValue();
  bool CheckStatus();

  void MakeConnections();

  void WriteSettingToADC();

  void SetValuesUI();

  bool SetSignalPeriod();
  bool SetADCDelay();
  bool SetSignalLength();
  bool SetUFOSAllRegimeReg();
  bool SetUFOSAllControlReg();
  bool SetADCControlReg();
  bool SetUFOSAllADCNumber();
  bool SetRegAF();

  void SetClockTune(uint8_t byte3, uint8_t byte2, uint8_t byte1, uint8_t byte0);
  void SetClockTune(uint32_t delay);
  void SetupAllDelayADC();

  void WriteDelayADC();
  void ReadDelayADC();

  void GetStatus();

  void SetCountAzimut(int azimut);

  //текущий ip адрес
  QString m_IpIndex = "192.168.0.2";
  //загрузка формы диоллогового окна
  Ui::UDPServerDialogBox *ui;

  //создание сетки
  QGridLayout *m_gridLayout;
  OldValues m_currentValues;
  // указатель модуль
  ADCModule *m_module;

  bool m_switchADCOnOff = false;
  bool m_switchUFOSOnOff = false;

  QMessageBox m_dialogErrors;
  std::vector<int32_t> m_delayADCBoard;
  NetworkUWB::SenderUDP m_senderToUDP;
  StatusADC m_statusADC;

  template <typename TypeData>
  void WriteRegisterADC(uint8_t addr, TypeData &data) {
    RegisterControlBoardsADC_ADDR registerControlBoardsADC_ADDR;
    registerControlBoardsADC_ADDR.SPI_ADDR =
        static_cast<uint8_t>(RegisterControlBoardsADC_ADDR::ADDR::ADDRESS_ADDR);
    registerControlBoardsADC_ADDR.SPI_DATA_ADC = addr;
    m_senderToUDP.SendData(registerControlBoardsADC_ADDR);

    RegisterControlBoardsADC_ADDR registerControlBoardsADC_DATA;
    registerControlBoardsADC_DATA.SPI_ADDR =
        static_cast<uint8_t>(RegisterControlBoardsADC_ADDR::ADDR::ADDRESS_DATA);
    registerControlBoardsADC_DATA.SPI_DATA_ADC =
        *(reinterpret_cast<uint8_t *>(&data));
    m_senderToUDP.SendData(registerControlBoardsADC_DATA);
  }

  template <typename TypeData> bool CheckData(uint8_t data, TypeData &regData) {
    uint8_t *ptrData = (uint8_t *)&regData;
    ptrData += 1;
    return (data == *(ptrData));
  }

  bool SetAccumMode();
};

#endif // ADCSERVERDIALOGBOX_H
