#include "serialport.h"
#include <iostream>

namespace UtilityUWB {
RSHandler::RSHandler() : m_serialPort(m_ioService) {}

RSHandler::~RSHandler() { Close(); }

bool RSHandler::Open(const std::string &dev) noexcept {
  ClearErrors();
  m_serialPort.open(dev, GetErrors());
  if (m_serialPort.is_open()) {
    m_serialPort.set_option(SerialPortBase::character_size(8));
    m_serialPort.set_option(
        SerialPortBase::flow_control(SerialPortBase::flow_control::none));
    m_serialPort.set_option(
        SerialPortBase::parity(SerialPortBase::parity::none));
    m_serialPort.set_option(
        SerialPortBase::stop_bits(SerialPortBase::stop_bits::one));
  }
  return IsNoError();
}

bool RSHandler::SetRate(uint rate) noexcept {
  ClearErrors();
  if (m_serialPort.is_open())
    m_serialPort.set_option(SerialPortBase::baud_rate(rate), GetErrors());
  return IsNoError();
}

bool RSHandler::Close() noexcept {
  ClearErrors();
  if (m_serialPort.is_open())
    m_serialPort.close(GetErrors());
  return IsNoError();
}

bool RSHandler::WriteCommand(const std::vector<uint8_t> &reqest) noexcept {
  ClearErrors();
  m_serialPort.write_some(boost::asio::buffer(reqest), GetErrors());
  return IsNoError();
}

bool RSHandler::GetAnswer(std::vector<uint8_t> &answer) noexcept {
  ClearErrors();
  // ожидание ответ в N байт
  size_t sizeReciveDelta = answer.size();
  while (sizeReciveDelta > 0) {
    if (!IsNoError())
      return false;

    size_t index = (answer.size() - sizeReciveDelta);
    size_t size = m_serialPort.read_some(
        boost::asio::buffer(&answer[index], sizeReciveDelta), GetErrors());
    sizeReciveDelta -= size;
    std::cout << "size recive - " << size << "; delta - " << sizeReciveDelta
              << std::endl;
  }
  std::cout << "================================" << std::endl;
  return IsNoError();
}

} // namespace UtilityUWB
