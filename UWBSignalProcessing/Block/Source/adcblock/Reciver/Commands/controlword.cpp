#include "controlword.h"
#include <iostream>

std::vector<uint8_t>& ControlWord::GetWord()
{
    AddCRC();
    return m_word;
}

void ControlWord::SetSync(uint8_t byte)
{
    m_word[NUMBER_BYTE::SYNC] = byte;
}

void ControlWord::SetAddr(uint8_t byte)
{
    m_word[NUMBER_BYTE::ADDR] = byte;
}

void ControlWord::SetCommand(uint8_t byte)
{
    m_word[NUMBER_BYTE::COMMAND] = byte;
    AddCRC();
}

void ControlWord::AddData(uint8_t byte)
{
    auto it = m_word.begin() + NUMBER_BYTE::DATA + SizeData();
    m_word.insert(it, byte);
    IncreaseSizeData(1);
    AddCRC();
}

void ControlWord::AddData(uint8_t byte1, uint8_t byte2)
{
    AddData(byte1);
    AddData(byte2);
}

void ControlWord::AddData(uint8_t byte1, uint8_t byte2, uint8_t byte3)
{
    AddData(byte1);
    AddData(byte2);
    AddData(byte3);
}

void ControlWord::AddData(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4)
{
    AddData(byte1);
    AddData(byte2);
    AddData(byte3);
    AddData(byte4);
}

uint8_t ControlWord::GetData(int index) const
{
    if(static_cast<size_t>(index) < SizeData())
        return m_word[NUMBER_BYTE::DATA + index];
    return 0xFF;
}

uint8_t& ControlWord::GetData(int index)
{
    return m_word[NUMBER_BYTE::DATA + index];
}

size_t ControlWord::SizeData() const
{
    return m_word[NUMBER_BYTE::SIZE_DATA] - 1;
}

bool ControlWord::CheckCRC() const
{
    return m_word.back() == CountCRC();
}

bool ControlWord::CheckCRC(uint8_t byte) const
{
    return byte == CountCRC();
}

void ControlWord::AddCRC()
{
    m_word.back() = CountCRC();
}

uint8_t ControlWord::CountCRC() const
{
    uint8_t crc = 0xFF;

    for (unsigned i = 3; i < m_word.size() - 1; i++)
    {
        uint8_t dat = m_word[i];

        for (unsigned j = 0; j < 8; j++)
        {
            uint8_t aux = (dat ^ crc) & 0x01;

            if(aux == 1)
                crc ^= 0x18;

            crc >>= 1;
            crc |= (aux << 7);
            dat >>= 1;
        }
    }
    return crc;
}

void ControlWord::IncreaseSizeData(uint8_t increment)
{
    m_word[NUMBER_BYTE::SIZE_DATA] += increment;
}

