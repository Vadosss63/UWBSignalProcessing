#ifndef READPATHSDIR_H
#define READPATHSDIR_H

#include <string>
#include <list>

// Функция получения всех файлов из указаной дериктории
// pathDirFile - путь к дериктории
// filter - фильтр возвращаемых файлов
// Если filter = "", то возвращаются все файлы
std::list<std::string> ReadPathFiles(const std::string& pathDirFile, const std::string& filter);

#endif // READPATHSDIR_H
