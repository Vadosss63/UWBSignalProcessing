#include "readpathsdir.h"
#include <boost/filesystem.hpp>

std::list<std::string> ReadPathFiles(const std::string& pathDirFile, const std::string& filter)
{
    namespace fs = boost::filesystem;
    std::list<std::string> listPath;
    fs::path dir = pathDirFile;
    for(fs::directory_iterator it(dir), end; it!= end; ++it)
    {
        if(filter.empty() || it->path().extension() == filter)
        {
            //            std::string p = it->path().c_str();
            std::string p = it->path().string();
            listPath.push_back(p);
        }
    }
    return listPath;
}
