#ifndef CRC16_H
#define CRC16_H

#include <QtGlobal>
#include <vector>

// CRC16
// Полином: 0x8005
// Начальное значение: 0xffff
quint16 crc16(const QByteArray &ba);
uint16_t crc16(const std::vector<uint8_t> &ba);

#endif // CRC16_H
