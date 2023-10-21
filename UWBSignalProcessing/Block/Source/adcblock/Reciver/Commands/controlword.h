#ifndef CONTROLWORD_H
#define CONTROLWORD_H

#include <stddef.h>
#include <stdint.h>
#include <vector>

enum COMMANDS {
  REBOOT = 0x10,
  SAVE = 0x11,
  GET_STATUS = 0x1E,
  GET_SETTING = 0x00,
  DM_AMP = 0x02,
  DM_AMP_2 = 0x1C,
  USP1Amp = 0x05,
  USP1Amp_2 = 0x16,
  USP2Amp = 0x06,
  USP2Amp_2 = 0x17,
  DMSynt = 0x14,
  SYNCRONIZATION = 0x0A,
  BAND = 0x0F,
  GET_STATUS_SYNCH = 0x29,
  ACCUM = 0x14,
  CONTROL_SCANE = 0x1D

};

// Формирует слово управления приемником

class ControlWord {
  enum NUMBER_BYTE { SYNC = 0, ADDR = 1, SIZE_DATA = 2, COMMAND = 3, DATA = 4 };

public:
  ControlWord() = default;
  ~ControlWord() = default;

  std::vector<uint8_t> &GetWord();

  // Установка байта синхронизации
  void SetSync(uint8_t byte);
  // Установка байта адреса
  void SetAddr(uint8_t byte);
  // Установка байта комманды
  void SetCommand(uint8_t byte);
  // Запись данных
  void AddData(uint8_t byte);
  void AddData(uint8_t byte1, uint8_t byte2);
  void AddData(uint8_t byte1, uint8_t byte2, uint8_t byte3);
  void AddData(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);
  // Получение данных из слова управления
  uint8_t GetData(int index = 0) const;
  // Возвращает метку size слова управления
  size_t SizeData() const;

  // Проверка контрольной суммы
  bool CheckCRC() const;
  bool CheckCRC(uint8_t byte) const;

  uint8_t &GetData(int index);

private:
  //Добавление контрольной суммы в слово управления
  void AddCRC();
  //Расчет контрольной суммы
  uint8_t CountCRC() const;
  // Увеличение байта метки size слова управления
  void IncreaseSizeData(uint8_t increment);
  // слово управления
  std::vector<uint8_t> m_word = {0xFF, 0xFF, 0x01, 0xFF, 0xFF};
};
#endif // CONTROLWORD_H
