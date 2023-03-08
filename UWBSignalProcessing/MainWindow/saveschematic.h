#ifndef SAVESCHEMATIC_H
#define SAVESCHEMATIC_H

#include <QFile>
#include <QString>
#include <QList>
#include <QMap>
#include <iostream>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "savestruct.h"


class PtreeWR
{
    typedef boost::property_tree::ptree Ptree;
public:
    PtreeWR();
    ~PtreeWR();

    // чтение дерева из стринга
    void ReadJson(std::string&& dataChar);
    void ReadJson(std::string&& dataChar, ulong activeBate);
    bool ReadFileJson(const std::string& pathFile);
    // выполняет запись в string-овый буфер
    std::string WriteToBuffer() const;
    void WriteToFile(const std::string& path);

    // Очистить дерево
    void Clear();

    void AddBlock(BlockStruct& blockStruct);
    QMap<size_t, BlockStruct> GetBlocks() const;

    void AddLink(LinkStruct linkStruct);
    QList<LinkStruct> GetLinks() const;

private:

    void WriteID(std::string id);
    void WriteType(std::string type, std::string idBlock);
    void WriteCoordX(std::string coordX, std::string idBlock);
    void WriteCoordY(std::string coordY, std::string idBlock);
    void WriteParametres(QList<std::pair<QString, QVariant> > listSetting, std::string idBlock);

    void AddParameterField(const std::string& pathValue, const std::string& val);
    std::string ReadParameterValue(const std::string& pathValue);

    // Обработчик JSON
    Ptree m_treeJson;
    Ptree m_links;

    // Используемые поля
    static constexpr const char*  m_textBlock = "Block";
    static constexpr const char*  m_textLink = "Link";

    static constexpr const char*  m_textID = "ID";
    static constexpr const char*  m_textType = "Type";
    static constexpr const char*  m_textCoordX = "coordX";
    static constexpr const char*  m_textCoordY = "coordY";
    static constexpr const char*  m_textParametres =  "Parametres";

    static constexpr const char*  m_textBlockIn = "BlockIn";
    static constexpr const char*  m_textNumberPortIn = "NumberPortIn";
    static constexpr const char*  m_textBlockOut = "BlockOut";
    static constexpr const char*  m_textNumberPortOut =  "NumberPortOut";
};

class SchemeWriter
{
public:

    SchemeWriter(const QString& fileName);
    ~SchemeWriter();
    void SaveBlock(BlockStruct blockStruct);
    void SaveLink(LinkStruct linkStruct);

private:
    QString m_fileName;
    PtreeWR m_ptreeWR;
};

class SchemeReader
{

public:
    SchemeReader();
    ~SchemeReader();
    bool ReadFile(const QString& fileName);
    QList<LinkStruct> Connects() const;
    QMap<size_t, BlockStruct> Blocks() const;

private:
    QString m_fileName;
    PtreeWR m_ptreeWR;
};

#endif // SAVESCHEMATIC_H
