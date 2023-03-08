#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <boost/asio.hpp>
#include "errorcodeboost.h"

namespace UtilityUWB
{

class RSHandler: public ErrorCodeBoost
{
    typedef boost::asio::serial_port SerialPort;
    typedef boost::asio::io_service IOservice;
    typedef boost::system::error_code ErrorCode;
    typedef boost::asio::serial_port_base SerialPortBase;

public:
    RSHandler();
    ~RSHandler();
    RSHandler(const RSHandler& rhs) = delete;
    RSHandler& operator=(const RSHandler& rhs) = delete;

    bool Open(const std::string &dev) noexcept;
    bool SetRate(uint rate = 115200) noexcept;
    bool Close() noexcept;

    bool WriteCommand(const std::vector<uint8_t> &reqest) noexcept;
    bool GetAnswer(std::vector<uint8_t>& answer) noexcept;

private:
    IOservice m_ioService;
    SerialPort m_serialPort;
};

}
#endif // SERIALPORT_H
