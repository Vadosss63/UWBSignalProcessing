#ifndef CONNECTPROTOCOL_H
#define CONNECTPROTOCOL_H

#include <cstdint>
namespace ConnectProtocol {

//Протокол пердачи данных состоит из
// 1 байт - тип команды
// 2-N байт - команда

// Команды передачи данных
enum class CMD : uint8_t {
  // Режим работы
  MODE_CONTROl = 0,
  // Версия СПО
  VERSION,
  // Изменение режима работы
  MODE_CHANGE,
  // Статус оборудования
  CHECK_STATUS,
  // Загрузка логов
  LOUD_LOGS,
  // Обновление СПО
  UPDATE_SPO,
  // Режим реального времени
  REALTIME_MODE,
  //Имитационный режим
  IMITATION_MODE,
  //Остановка отладочных режимов
  STOP_MODE
};
// Статусы приемника
enum class ReciverStatus : uint8_t { NO_GRAG = 0, OK, NO_CONNECT };
// Статусы синхронизатора
enum class SynchronizationStatus : uint8_t { NO_GRAG = 0, OK, NO_CONNECT };
// Статусы АЦП
enum class ADCStatus : uint8_t { WORK = 0, NO_WORK, NO_CONNECT };
// Режим соединения
enum class ModeConnection : uint8_t {
  // Штатный
  Normal = 0,
  Debug,
  Technological,
  NoConnect,
  Error
};
// Статус обновления СПО
enum class StatusUpdateSPO : uint8_t { OK = 0, Error };
// Версия ПО
struct VERSION_SPO {
  uint8_t major;
  uint8_t minor;
} __attribute__((packed));
// Статус оборудования
struct STATUS_HARDWARE {
  SynchronizationStatus synchronizationStatus =
      SynchronizationStatus::NO_CONNECT;
  ADCStatus adc_Status = ADCStatus::NO_CONNECT;
  ReciverStatus reciverStatus = ReciverStatus::NO_CONNECT;
} __attribute__((packed));
// Режим реального времи
struct RealTimeMode {
  int X_begin = 0;
  int X_end = 0;
  int Y_begin = 0;
  int Y_end = 0;
} __attribute__((packed));
// Имитационный режим
struct ImitationMode {
  // Координаты X и Y
  int X = 0;
  int Y = 0;
  // Скорость км/ч
  int V = 0;
} __attribute__((packed));
} // namespace ConnectProtocol

#endif // CONNECTPROTOCOL_H
