#ifndef FILETRNETWORK_H
#define FILETRNETWORK_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <iostream>
#include <fstream>
#include <boost/enable_shared_from_this.hpp>

namespace FileTRNetwork
{

using boost::asio::ip::tcp;
// максимальное количество символов в пути к файлу
constexpr static size_t MAX_PATH_CHAR = 30;

struct HeaderFile
{
    // Размер файла
    size_t sizeFile;
    // Количество оставшихся файлов
    size_t countFiles;
    // Соличество символов в названии
    size_t countChar;
}__attribute__ ((packed));

class FileSender
{

public:

    FileSender(boost::asio::io_service& io_service);
    // Установка адреса получателя
    void SetAddr(const std::string& ip, unsigned short port);
    // Устанавливаем путь к файлу для отправки
    void SetFilePath(const std::string& path);
    // Устанавливаем список передоваемых файлов
    void SetFilePaths(std::list<std::string>&& paths);
    bool InitConnection();

private:

    // Установка файла для отправки данных
    bool SetupFile();
    // Функция соедиения с получателем
    void HandleResolve(const boost::system::error_code & err, tcp::resolver::iterator endpoint_iterator);
    // Функция синхронной записи в сокет
    void HandleConnect(const boost::system::error_code &err, tcp::resolver::iterator endpoint_iterator);
    // Выполняет чтение файла и запись его в сокет
    void HandleWriteFile(const boost::system::error_code& err);

    // адрес получателя
    tcp::resolver::endpoint_type m_ep;
    tcp::resolver m_resolver;
    tcp::socket m_socket;
    // поток чтения файлов
    std::ifstream m_sourceFile;
    //Отправляеммые файлы
    std::list<std::string> m_paths;
    // бефер для отправки данных через сокет
    boost::array<char, 1024> m_buf;
    void SendRequestReciveFile();
};

class FileReciver
{

public:

    FileReciver(unsigned short port);
    ~FileReciver();
    // Запуск приема данных
    void Start();
    // Остановка приема данных
    void Stop();
    // Установка папки загрузки
    void SetPathDir(const std::string& pathDir);
    // Подключение входящего соединения
    void HandleAccept(const boost::system::error_code& e);
    // устанавливает функцию которую надо
    // вызвать при завершении передачи данных
    // Приннимает на вход колбек
    void AddCallback(const std::function<void()>& callback);

private:

    void DeleteConnections();

    // Запуск соединения
    void StartConnect();
    // функция обработки входящего запроса на запись файла
    void HandleReadRequest(const boost::system::error_code& err, std::size_t bytes_transferred);
    // функция записи принимаемых данных в файл
    void HandleReadFileContent(const boost::system::error_code& err, std::size_t bytes_transferred);
    // функция обработки ошибок
    void HandleError(const std::string& function_name, const boost::system::error_code& err);
    // установка файла для записи
    bool SetupFile(const std::string& path);
    // асинхронное чтение файла
    void AsyncRead(size_t size);
    void InitConnections();

    // сокет для соединения с отпраавителем
    boost::asio::io_service* m_IOservice = nullptr;
    boost::asio::ip::tcp::acceptor* m_acceptor = nullptr;
    boost::asio::ip::tcp::socket* m_socket = nullptr;

    unsigned short m_port;

    // Путь к принимаемым файлам
    std::string m_pathDir = "";
    // поток для записи принимаемого файла
    std::ofstream m_outputFile;
    // размер принимаемого файла
    size_t m_fileSize = 0;
    // количество которые еще необходимо принять файлов
    size_t m_countFiles = 0;
    // буфер для чтения принимаемых данных
    // boost::array<char, 40960> m_buf;
    constexpr static size_t MAX_SIZE_BUF = 1024;
    boost::array<char, MAX_SIZE_BUF> m_buf;
    // колбек вызываемый при успешной записи файла
    std::function<void()> m_callback = [](){};
};

// Оставляем только название файла
std::string TrimPathFile(const std::string& path);

}
#endif // FILETRNETWORK_H
