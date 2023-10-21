#ifndef ADCPROTOCOL_H
#define ADCPROTOCOL_H

#include <cstdint>

struct DataADC {
  // номер команды
  uint8_t command;
  // Номер кадра в стробе
  uint8_t NFrame;
  // Всего кадров в стробе
  uint8_t TFrame;
  // Номер строба
  uint8_t STBNum;
  // Номер угла
  uint8_t AngelNum;
  // Данные АЦП
  int16_t data[512];
} __attribute__((packed));

struct SetAdcModePackage {
  // Set Command code 0x06
  uint8_t command;
  // sync mode
  uint8_t syncMode;
  // adc mode
  uint8_t adcMode;
  // sample count
  uint8_t sampleNum;
  // aver num
  uint8_t averNum;
  // aver num
  uint8_t averMode;
  // делитель накопления
  uint8_t averDiv = 0;
} __attribute__((packed));

struct AdcStartPackage {
  // set command 0x03
  uint8_t command;
  // run or stop adc board
  uint8_t runAdc;
} __attribute__((packed));

enum class ADC_COMMANDS : uint8_t {
  ADC_CMD_GET_STATUS = 0x01,
  ADC_CMD_GET_ECHO = 0x02,
  ADC_CMD_START = 0x03,
  ADC_CMD_GET_START = 0x04,
  ADC_CMD_GET_DATA = 0x05,
  ADC_CMD_SET_MODE = 0x06,
  ADC_CMD_READY = 0x07,
  ADC_CMD_STREAM = 0x08
};

enum class ADC_START_STOP_CMD : uint8_t {
  ADC_STOP_COMMAND = 0x00,
  ADC_START_COMMAND = 0x01
};

enum class ADC_INFO {
  ADC_DATA_PACKAGE_HEADER_LEN = 6,

  ADC_CMD_FRAME_SIZE = 61,

  ADC_BUF_SIZE = 2048,
  ADC_FRAME_SIZE = 1024,
  ADC_FIFO_SIZE = 2048,
  ADC_STROBE_SIZE = 8192,
  N_ATTEMPT = 4,

  ADC_STROBE_FRAMES_COUNT = 32,

  ADC_SYNC_BYTE_DOWN = 0xAA,
  ADC_SYNC_BYTE_UP = 0x55,

  ERROR_NO = 0,
  ERROR_FS = -1,
  ERROR_PPM = -2,

  HOST_MAC_0 = 0x61,
  HOST_MAC_1 = 0xD8,
  HOST_MAC_2 = 0xE3,
  HOST_MAC_3 = 0x77,
  HOST_MAC_4 = 0x13,
  HOST_MAC_5 = 0x00,

  ADC_MAC_0 = 0x01,
  ADC_MAC_1 = 0x40,
  ADC_MAC_2 = 0xAE,
  ADC_MAC_3 = 0xC2,
  ADC_MAC_4 = 0x50,
  ADC_MAC_5 = 0x00
};

enum class ADC_SYNC : uint8_t {
  SYNC_MODE_INT = 0x00, // Внутренняя синхронизация
  SYNC_MODE_EXT = 0x01, // Внешняя синхронизация
  SYNC_MODE_TST = 0x03  // Тестовая синхронизация
};

enum class ADC_MODE : uint8_t {
  ADC_NORM_MODE = 0x00, //Нормальный режим работы
  ADC_TPTN_MODE = 0x01, // тестовые данные
  ADC_SINE_MODE = 0x02, // Sine режим (Не реализован)
  ADC_PD_MODE = 0x03    // Powerdown
};

enum class AVER_MODE : uint8_t {
  AVER_OFF = 0x00, // нет усреднения
  AVER_ON = 0x01   // работаем с усреднением
};

enum class SAMPLE_NUM : uint8_t {
  SAMP_512 = 0x01,  // 512 отсчета
  SAMP_1024 = 0x02, // 1024 отсчета
  SAMP_2048 = 0x03, // 2048 отсетов
  SAMP_3072 = 0x04, // 3072 отсчета
  SAMP_4096 = 0x05, // 4096 отсчетов
  SAMP_5120 = 0x06, // 5120 отсчетов
  SAMP_6144 = 0x07, // 6140 отсчетов
  SAMP_7168 = 0x08  // 7168 отсчетов
};

enum class AVER_DIV : uint8_t {
  DIV_1 = 0x00,   // усреднение по 1
  DIV_2 = 0x01,   // усреднение по 2
  DIV_4 = 0x02,   // усреднение по 4
  DIV_8 = 0x03,   // усреднение по 8
  DIV_16 = 0x04,  // усреднение по 16
  DIV_32 = 0x05,  // усреднение по 32
  DIV_64 = 0x06,  // усреднение по 64
  DIV_128 = 0x07, // усреднение по 128
  DIV_256 = 0x08  // усреднение по 256
};

enum class AVER_NUM : uint8_t {
  AVER_2 = 0x01,   // усреднение по 2
  AVER_4 = 0x02,   // усреднение по 4
  AVER_8 = 0x04,   // усреднение по 8
  AVER_16 = 0x08,  // усреднение по 16
  AVER_32 = 0x10,  // усреднение по 32
  AVER_64 = 0x20,  // усреднение по 64
  AVER_128 = 0x40, // усреднение по 128
  AVER_256 = 0x80  // усреднение по 256

};

//  Протокол передачи данных в uart через АЦП
namespace UART_UWB {

// протокол передачи данных состоит:
// uint8_t header - заголовок
// uint8_t size - длина пакета данных
// uint8_t data[size] - данные

// uint8_t data[size] - данные команды могут иметь максимальный размер size =
// 16; data состоит uint8_t cmd(data[0]) - пользовательская команда uint8_t
// data[1 ... (size - 2)] - данные в рамках команды uint8_t crc(data[size -1]) -
// контрольная сумма, вычисляется простым сумированием всего масиива данных
// кроме последнего

// Заголовачный байт
static constexpr uint8_t header = 0x11;

struct CMD_HARDWARE {
  uint8_t h = 0x11;
  uint8_t size = 0x02;
  uint8_t cmd = 0x29;
  uint8_t crc = 0x29;
} __attribute__((packed));

struct CMD_HARDWARE_answer {
  uint8_t h;
  uint8_t size;
  uint8_t cmd;
  uint8_t sync : 1;
  uint8_t dataX2 : 2;
  uint8_t rec : 1;
  uint8_t dataX1 : 4;
  uint8_t crc;
} __attribute__((packed));

struct IM_CMD {
  uint8_t h = 0x11;
  uint8_t size = 0x07;
  uint8_t cmd = 0x33;
  uint16_t d = 0x00;
  int8_t t = 0x00;
  int8_t v = 0x00;
  int8_t start = 0x01; // 0x09;
  uint8_t crc = 0x00;
  static constexpr uint8_t ONE_RUN = 0x09;
  static constexpr uint8_t CIRCLE_RUN = 0x01;
} __attribute__((packed));

// Запуск/Отключение синхронизатора
class EnableSyncCMD {

public:
  void SetEnable() {
    m_data = ENABLE;
    AddCRC();
  }
  void SetDisable() {
    m_data = DISABLE;
    AddCRC();
  }

private:
  void AddCRC() { m_crc = m_cmd + m_data; }

  static constexpr uint8_t ENABLE = 0x01;
  static constexpr uint8_t DISABLE = 0x00;
  uint8_t m_h = 0x11;
  uint8_t m_size = 0x03;
  uint8_t m_cmd = 0x1C;
  uint8_t m_data = ENABLE;
  uint8_t m_crc = 0x1D;
} __attribute__((packed));

// имитируемая цель
struct ImitationTarrget {
  int16_t x;
  int16_t y;
  // скорость
  int16_t v;
} __attribute__((packed));

// Нормальный режим
struct IM_Start {
  uint8_t h = 0x11;
  uint8_t size = 0x03;
  uint8_t cmd = 0x34;
  int8_t d = 0x00;
  uint8_t crc = 0x34;
} __attribute__((packed));

} // namespace UART_UWB

#endif // ADCPROTOCOL_H
