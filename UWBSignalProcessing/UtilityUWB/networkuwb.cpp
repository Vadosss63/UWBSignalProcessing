#include "networkuwb.h"

namespace NetworkUWB {

SenderUDP::SenderUDP() : m_sock(m_service) {}

SenderUDP::SenderUDP(uint16_t port) : m_sock(m_service) {
  SetEndPoint("127.0.0.1", port);
  Open();
}

SenderUDP::SenderUDP(const std::string &address, uint16_t port)
    : m_sock(m_service) {
  SetEndPoint(address, port);
  Open();
}

SenderUDP::~SenderUDP() { Close(); }

void SenderUDP::Close() {
  if (m_sock.is_open())
    m_sock.close();
}

bool SenderUDP::Open() noexcept {
  Close();
  ClearErrors();
  m_sock.open(Udp::v4(), GetErrors());
  return IsNoError();
}

void SenderUDP::SetEndPoint(const std::string &adress, uint16_t port) noexcept {
  m_ep = EndPoint(Address::from_string(adress), port);
}

bool SenderUDP::SendData(const std::string &data) noexcept {
  ClearErrors();
  m_sock.send_to(boost::asio::buffer(data), m_ep, 0, GetErrors());
  return IsNoError();
}

bool SenderUDP::SendData(const uint8_t *data, size_t size) noexcept {
  ClearErrors();
  m_sock.send_to(boost::asio::buffer(data, size), m_ep, 0, GetErrors());
  return IsNoError();
}

bool SenderUDP::SendData(const boost::asio::streambuf &buf) noexcept {
  ClearErrors();
  m_sock.send_to(buf.data(), m_ep, 0, GetErrors());
  return IsNoError();
}

bool SenderUDP::ReadWithTimeout(const Timeout &expiryTime) {
  ClearErrors();
  boost::optional<ErrorCode> timerResult;
  boost::asio::deadline_timer timer(m_service);
  timer.expires_from_now(expiryTime);

  m_sizeRecivedByte = 0;
  timer.async_wait([&](const ErrorCode &error) { timerResult.reset(error); });

  boost::optional<ErrorCode> readResult;

  m_sock.async_receive(
      boost::asio::buffer(m_bufferReciveData.Data(), m_bufferReciveData.Size()),
      [&](const ErrorCode &error, size_t s) {
        m_sizeRecivedByte = s;
        readResult.reset(error);
        if (error)
          GetErrors() = readResult.value();
      });

  m_service.reset();
  while (m_service.run_one()) {
    if (readResult) {
      timer.cancel();
    } else if (timerResult) {
      m_sock.cancel();
      GetErrors().assign(boost::system::errc::timed_out, *(new ErrorTimout()));
    }
  }
  return IsNoError();
}

std::string SenderUDP::GetData() const {
  std::string data(reinterpret_cast<char *>(m_bufferReciveData.Data()),
                   m_sizeRecivedByte);
  return data;
}

std::pair<uint8_t *, size_t> SenderUDP::GetData_uint8() const {
  uint8_t *data = reinterpret_cast<uint8_t *>(malloc(m_sizeRecivedByte));
  memcpy(data, m_bufferReciveData.Data(), m_sizeRecivedByte);
  std::pair<uint8_t *, size_t> returnData(data, m_sizeRecivedByte);
  return returnData;
}

bool SenderUDP::Bind(uint16_t port) noexcept {
  using namespace boost::placeholders;
  EndPoint ep(Udp::v4(), port);
  ClearErrors();
  m_sock.bind(ep, GetErrors());
  return IsNoError();
}

//////////////////////////////////////////////
/// \brief ReciverUDP::ReciverUDP
/////////////////////////////////////////////

ReciverUDP::ReciverUDP()
    : m_service(new IOservice), m_socket(new Socket(*m_service)),
      m_isStart(false) {}

ReciverUDP::ReciverUDP(uint16_t port)
    : m_service(new IOservice), m_socket(new Socket(*m_service)),
      m_isStart(false) {
  SetEndPoint("127.0.0.1", port);
}

ReciverUDP::ReciverUDP(const std::string &address, uint16_t port)
    : m_service(new IOservice), m_socket(new Socket(*m_service)),
      m_isStart(false) {
  SetEndPoint(address, port);
}

ReciverUDP::ReciverUDP(const std::string &address, uint16_t port,
                       uint16_t sizeBuf)
    : m_service(new IOservice), m_socket(new Socket(*m_service)),
      m_isStart(false), m_bufferReciveData(sizeBuf) {
  SetEndPoint(address, port);
}

ReciverUDP::~ReciverUDP() {
  Stop();
  delete m_socket;
  delete m_service;
}

void ReciverUDP::SetEndPoint(const std::string &address,
                             uint16_t port) noexcept {
  m_epReciver = EndPoint(Address_v4::from_string(address), port);
}

bool ReciverUDP::IsStart() noexcept { return m_isStart.load(); }

bool ReciverUDP::OpenSocket() noexcept {
  ClearErrors();
  if (m_socket->is_open())
    m_socket->close();
  m_socket->open(m_epReciver.protocol(), GetErrors());
  return IsNoError();
}

bool ReciverUDP::Bind() noexcept {
  using namespace boost::placeholders;
  ClearErrors();
  m_socket->bind(m_epReciver, GetErrors());
  return IsNoError();
}

bool ReciverUDP::Start() {
  if (IsStart())
    return true;

  ClearErrors();
  // Открываем сокет и биндимся
  if (OpenSocket() && Bind()) {
    m_socket->set_option(boost::asio::ip::udp::socket::reuse_address(true));
    m_thread = std::thread(&ReciverUDP::ThreadReceiver, this);
  }
  return IsNoError();
}

void ReciverUDP::Stop() {
  m_isStart.store(false);
  m_service->stop();

  // корректное завершения потока
  if (m_thread.joinable())
    m_thread.join();

  if (m_socket->is_open()) {
    // m_socket->shutdown(boost::asio::ip::udp::socket::shutdown_receive);
    m_socket->close();
  }
  delete m_socket;
  delete m_service;
  m_service = new IOservice;
  m_socket = new Socket(*m_service);
}

std::string ReciverUDP::GetData() const {
  std::string data(reinterpret_cast<char *>(m_bufferReciveData.Data()),
                   m_sizeRecivedByte);
  return data;
}

std::pair<uint8_t *, size_t> ReciverUDP::GetData_uint8() const {
  uint8_t *data = reinterpret_cast<uint8_t *>(malloc(m_sizeRecivedByte));
  memcpy(data, m_bufferReciveData.Data(), m_sizeRecivedByte);
  std::pair<uint8_t *, size_t> returnData(data, m_sizeRecivedByte);
  return returnData;
}

void ReciverUDP::ThreadReceiver() {
  AsyncRecive();
  m_isStart.store(true);
  //    m_service.reset();
  m_service->run();
}

void ReciverUDP::AsyncRecive() {
  m_socket->async_receive_from(
      boost::asio::buffer(m_bufferReciveData.Data(), m_bufferReciveData.Size()),
      m_epSender,
      [&](const ErrorCode &error, size_t s) { ReadData(error, s); });
}

void ReciverUDP::ReadData(const ErrorCode &ec, size_t bytesTransferred) {
  if (ec) // проверяем сокет на ошибки
  {
    ClearErrors();
    GetErrors() = ec;
    return;
  }
  m_sizeRecivedByte = bytesTransferred;
  if (m_sizeRecivedByte)
    m_callback();

  // принимаем данные из сокета ко
  if (IsStart())
    AsyncRecive();
}

bool ReciverUDP::SendAnswer(const std::string &data) noexcept {
  ClearErrors();
  m_socket->send_to(boost::asio::buffer(data), m_epSender, 0, GetErrors());
  return IsNoError();
}

bool ReciverUDP::SendAnswer(uint8_t *data, size_t size) noexcept {
  ClearErrors();
  m_socket->send_to(boost::asio::buffer(data, size), m_epSender, 0,
                    GetErrors());
  return IsNoError();
}

bool ReciverUDP::SetMulticastMode() noexcept {
  ClearErrors();
  m_socket->set_option(
      boost::asio::ip::multicast::join_group(m_epReciver.address()),
      GetErrors());
  return IsNoError();
}

void ReciverUDP::CreateBuffer(uint16_t size) {
  m_bufferReciveData.Resize(size);
}

BufferReciveData::BufferReciveData() { Resize(m_sizeBufferRecive); }

BufferReciveData::BufferReciveData(size_t size) { Resize(size); }

BufferReciveData::~BufferReciveData() { DeleteBuff(); }

uint8_t *BufferReciveData::Data() const { return m_bufferRecive; }

size_t BufferReciveData::Size() const { return m_sizeBufferRecive; }

void BufferReciveData::Resize(size_t newSize) {
  DeleteBuff();
  m_sizeBufferRecive = newSize;
  m_bufferRecive = new uint8_t[m_sizeBufferRecive];
}

void BufferReciveData::DeleteBuff() {
  delete[] m_bufferRecive;
  m_bufferRecive = nullptr;
}
} // namespace NetworkUWB
