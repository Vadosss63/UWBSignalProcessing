#ifndef DATAGRAMKAMAZPRLI_H
#define DATAGRAMKAMAZPRLI_H

#include <functional>
#include "csignal.h"

namespace KamazDatagram
{

static const size_t maxSize = 1024; // пакеты предаются по 1024 для удобства расчета

struct HeaderPRLI
{
    // количество пакетов
    uint32_t countPackage:8;
    // идентификатор сообщения
    uint32_t idPackage:8;
    // Количество значимых дискретов
    uint32_t significantNumberOfDiscrete:16;
    // Циклический номер сообщения
    uint32_t cyclicMessageNumber;
    // Дальность первого элемента массива
    uint32_t rangeOfFirstElement;
    // Пелинг
    uint32_t bearing:8;
    // Резерв
    uint32_t x:24;
};

struct DataPRLI
{
    // Массив ПРЛИ
    uint16_t data[maxSize/sizeof(uint16_t)];
};

#pragma pack(push, 1)
struct PackageKamaz
{
    HeaderPRLI headerPRLI;
    DataPRLI dataPRLI;
};
#pragma pack(pop)

} // namespace KamazDatagram

#endif // DATAGRAMKAMAZPRLI_H
