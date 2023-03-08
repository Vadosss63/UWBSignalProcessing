#ifndef RS485HANDLER_H
#define RS485HANDLER_H

#include <vector>

#include "serialport.h"
#include "interfaces.h"

class RS485Handler: public HandlerDevice
{    

public:
    RS485Handler();
    ~RS485Handler() override;
    RS485Handler(const RS485Handler& rhs) = delete;
    RS485Handler& operator=(const RS485Handler& rhs) = delete;
    // открытие целевого устройства
    bool Open(const std::string &dev) override;
    // выполнение команды управления
    bool ExicuteCommand(Command *command) override;
    // возвращение ошибки
    std::string GetError() override;
    bool IsNoError() override;

private:
    bool WriteCommand(const std::vector<uint8_t> &reqest);
    bool GetAnswer(std::vector<uint8_t>& answer);

    UtilityUWB::RSHandler m_rs485Port;
};

#endif // RS485HANDLER_H
