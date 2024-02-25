#include "filetrnetwork.h"
namespace FileTRNetwork {

FileSender::FileSender(boost::asio::io_service &io_service)
    : m_resolver(io_service), m_socket(io_service) {}

void FileSender::SetFilePath(const std::string &path) {
  m_paths.clear();
  m_paths.push_back(path);
}

void FileSender::SetFilePaths(std::list<std::string> &&paths) {
  m_paths = std::move(paths);
}

bool FileSender::InitConnection() {
  if (!SetupFile())
    return false;
  m_resolver.async_resolve(m_ep,
                           [&](const boost::system::error_code &err,
                               tcp::resolver::iterator endpoint_iterator) {
                             HandleResolve(err, endpoint_iterator);
                           });
  return true;
}

void FileSender::SetAddr(const std::string &ip, unsigned short port) {
  m_ep = tcp::resolver::endpoint_type(
      boost::asio::ip::address_v4::from_string(ip), port);
}

bool FileSender::SetupFile() {
  if (m_paths.empty())
    return false;
  // Получаем первый файл для отправки
  std::string path = m_paths.front();
  // удаляем его из очереди
  m_paths.pop_front();

  // Если файл открыт закрываем его
  if (m_sourceFile.is_open())
    m_sourceFile.close();

  // открываем файл для отправки
  m_sourceFile.open(path, std::ios_base::binary | std::ios_base::ate);
  // проверяем на ошибки
  if (!m_sourceFile)
    return false;
  // получаем размер файла
  size_t fileSize = m_sourceFile.tellg();
  // переводим указатель чтения файла на 0
  m_sourceFile.seekg(0);
  // выполняем передачу данных
  // Записываем путь к файлу и размер файла и количество оставшихся файлов
  size_t countFiles = m_paths.size();
  HeaderFile *hf = reinterpret_cast<HeaderFile *>(m_buf.c_array());
  hf->sizeFile = fileSize;
  hf->countFiles = countFiles;
  std::string fileName = TrimPathFile(path);
  hf->countChar = fileName.size() + 1;
  char *cPath = m_buf.c_array();
  cPath += sizeof(HeaderFile);
  memcpy(cPath, fileName.c_str(), fileName.size() + 1);
  return true;
}

void FileSender::HandleResolve(const boost::system::error_code &err,
                               tcp::resolver::iterator endpoint_iterator) {
  if (!err) {
    tcp::endpoint endpoint = *endpoint_iterator;
    m_socket.async_connect(endpoint,
                           boost::bind(&FileSender::HandleConnect, this,
                                       boost::asio::placeholders::error,
                                       ++endpoint_iterator));
  } else {
    std::cout << "Error: " << err.message() << std::endl;
  }
}

void FileSender::SendRequestReciveFile() {
  boost::asio::async_write(
      m_socket,
      boost::asio::buffer(m_buf.c_array(), MAX_PATH_CHAR + sizeof(HeaderFile)),
      boost::bind(&FileSender::HandleWriteFile, this,
                  boost::asio::placeholders::error));
}

void FileSender::HandleConnect(const boost::system::error_code &err,
                               tcp::resolver::iterator endpoint_iterator) {
  if (!err) {
    SendRequestReciveFile();
  } else if (endpoint_iterator != tcp::resolver::iterator()) {
    m_socket.close();
    tcp::endpoint endpoint = *endpoint_iterator;
    m_socket.async_connect(endpoint,
                           boost::bind(&FileSender::HandleConnect, this,
                                       boost::asio::placeholders::error,
                                       ++endpoint_iterator));
  } else {
    std::cout << "Error: " << err.message() << std::endl;
  };
}

void FileSender::HandleWriteFile(const boost::system::error_code &err) {
  if (!err) {
    if (m_sourceFile) {
      // читаем данные в буфер
      m_sourceFile.read(m_buf.c_array(), (std::streamsize)m_buf.size());
      // проверяем сколько данных прочитано
      if (m_sourceFile.gcount() <= 0) {
        std::cout << "read file error" << std::endl;
        return;
      };

      std::cout << "Send " << m_sourceFile.gcount()
                << " bytes, total: " << m_sourceFile.tellg() << " bytes.\n";

      // отправляем данные полуателю
      boost::asio::async_write(
          m_socket, boost::asio::buffer(m_buf.c_array(), m_sourceFile.gcount()),
          boost::bind(&FileSender::HandleWriteFile, this,
                      boost::asio::placeholders::error));
    } else {
      if (SetupFile()) {
        std::cout << "Send next file! " << std::endl;
        SendRequestReciveFile();
        return;
      }
      std::cout << "End Write File " << std::endl;
      return;
    }
  } else {
    std::cout << "Error: " << err.message() << std::endl;
  }
}

void FileReciver::StartConnect() {
  // читаем заголовок
  m_socket->async_receive(
      boost::asio::buffer(m_buf.c_array(), MAX_PATH_CHAR + sizeof(HeaderFile)),
      [&](const boost::system::error_code &error, size_t s) {
        HandleReadRequest(error, s);
      });
}

void FileReciver::AddCallback(const std::function<void()> &callback) {
  m_callback = callback;
}

void FileReciver::DeleteConnections() {
  if (m_socket)
    delete m_socket;

  if (m_acceptor)
    delete m_acceptor;

  if (m_IOservice)
    delete m_IOservice;
}

void FileReciver::AsyncRead(size_t size) {
  m_socket->async_receive(boost::asio::buffer(m_buf.c_array(), size),
                          [&](const boost::system::error_code &error,
                              size_t s) { HandleReadFileContent(error, s); });
}

void FileReciver::HandleReadRequest(const boost::system::error_code &err,
                                    std::size_t bytes_transferred) {
  if (err)
    return HandleError(__FUNCTION__, err);

  if (bytes_transferred == 0)
    return;
  // Получаем заголовок
  HeaderFile *hf = reinterpret_cast<HeaderFile *>(m_buf.c_array());
  m_fileSize = hf->sizeFile;
  m_countFiles = hf->countFiles;
  char *pathChar = m_buf.c_array();
  pathChar += sizeof(HeaderFile);

  std::string filePath = pathChar;
  std::cout << "Recive File: " + filePath << std::endl;
  std::cout << "Size File: " << m_fileSize << std::endl;
  std::cout << "No send files: " << m_countFiles << std::endl;
  if (!SetupFile(filePath))
    return;

  // определяем размер буффера для приема файла
  size_t sizeBuffer = m_fileSize > MAX_SIZE_BUF ? MAX_SIZE_BUF : m_fileSize;
  AsyncRead(sizeBuffer);
}

void FileReciver::HandleReadFileContent(const boost::system::error_code &err,
                                        std::size_t bytes_transferred) {
  if (bytes_transferred > 0) {
    // записываем принятые данные в файловый поток
    m_outputFile.write(m_buf.c_array(), (std::streamsize)bytes_transferred);
  }

  if (err)
    return HandleError(__FUNCTION__, err);

  // Количество записанных байт в файл
  size_t sizeCurrent = static_cast<size_t>(m_outputFile.tellp());
  if (sizeCurrent >= m_fileSize) // проверяем приняты ли все файлы
  {
    std::cout << "Read byte = " << sizeCurrent << std::endl;
    // Закрываем файл
    m_outputFile.close();
    if (m_countFiles) {
      StartConnect();
      return;
    }
    std::cout << "End Recive !!!" << std::endl;
    m_callback();
    return;
  } else {
    size_t sizeBuffer = (m_fileSize - sizeCurrent) > MAX_SIZE_BUF
                            ? MAX_SIZE_BUF
                            : (m_fileSize - sizeCurrent);
    AsyncRead(sizeBuffer);
  }
}

void FileReciver::HandleError(const std::string &function_name,
                              const boost::system::error_code &err) {
  std::cout << __FUNCTION__ << " in " << function_name << " due to " << err
            << " " << err.message() << std::endl;
}

bool FileReciver::SetupFile(const std::string &path) {
  if (m_outputFile.is_open())
    m_outputFile.close();

  std::string filePath = TrimPathFile(path);

  // выполняем установку пути файла
  filePath = m_pathDir + filePath;
  m_outputFile.open(filePath, std::ios_base::binary);
  if (!m_outputFile) {
    std::cout << __LINE__ << "Failed to open: filePath_path" << std::endl;
    return false;
  }
  return true;
}

FileReciver::FileReciver(unsigned short port) : m_port(port) {}

void FileReciver::HandleAccept(const boost::system::error_code &e) {
  std::cout << __FUNCTION__ << " " << e << ", " << e.message() << std::endl;

  if (!e)
    StartConnect();
}

void FileReciver::Start() {
  DeleteConnections();
  InitConnections();
  m_IOservice->run();
}

void FileReciver::SetPathDir(const std::string &pathDir) {
  m_pathDir = pathDir;
}

void FileReciver::InitConnections() {
  m_IOservice = new boost::asio::io_service;

  m_acceptor = new boost::asio::ip::tcp::acceptor(
      *m_IOservice,
      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_port), true);

  m_socket = new boost::asio::ip::tcp::socket(*m_IOservice);

  // Устанавливаем колбек для прослушки входного соединения
  m_acceptor->async_accept(
      *m_socket, [&](const boost::system::error_code &e) { HandleAccept(e); });
}

void FileReciver::Stop() {
  if (m_IOservice)
    m_IOservice->stop();
}

FileReciver::~FileReciver() {
  Stop();
  DeleteConnections();
}

std::string TrimPathFile(const std::string &path) {
  std::string fileName = path;
  // ищим последний слеш чтобы получить только имя файла
  size_t pos = fileName.find_last_of('\\');
  size_t posLinux = fileName.find_last_of('/');

  if (pos != std::string::npos)
    fileName = fileName.substr(pos + 1);

  if (posLinux != std::string::npos)
    fileName = fileName.substr(posLinux + 1);

  return fileName;
}
} // namespace FileTRNetwork
