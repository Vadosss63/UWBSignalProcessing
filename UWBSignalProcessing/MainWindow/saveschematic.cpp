#include "saveschematic.h"

PtreeWR::PtreeWR(){}

PtreeWR::~PtreeWR()
{
    m_treeJson.clear();
}

void PtreeWR::ReadJson(std::string&& data)
{
    m_treeJson.clear();
    std::istringstream streamBuf(std::move(data));
    boost::property_tree::read_json(streamBuf, m_treeJson);
}

void PtreeWR::ReadJson(std::string&& dataChar, ulong activeBate)
{
    m_treeJson.clear();
    std::string data(dataChar.begin(), dataChar.begin() + activeBate);
    std::istringstream streamBuf(data);
    boost::property_tree::read_json(streamBuf, m_treeJson);
}

bool PtreeWR::ReadFileJson(const std::string& pathFile)
{
    m_treeJson.clear();
    try
    {
        boost::property_tree::read_json(pathFile, m_treeJson);
        return true;
    } catch (...)
    {
        return false;
    }

}

std::string PtreeWR::WriteToBuffer() const
{
    std::ostringstream streamBuf;
    boost::property_tree::write_json(streamBuf, m_treeJson);
    return streamBuf.str();
}

void PtreeWR::WriteToFile(const std::string& path)
{
    try
    {
        std::string textpath = m_textLink;
        m_treeJson.add_child(textpath, m_links);
        boost::property_tree::write_json(path, m_treeJson);
    }
    catch (...){}
}

void PtreeWR::WriteID(std::string id)
{
    std::string pathId = std::string(m_textBlock) + "." + id + "." + std::string(m_textID);
    AddParameterField(pathId , id);
}

void PtreeWR::WriteType(std::string type, std::string idBlock)
{
    std::string path = std::string(m_textBlock) + "." + idBlock + "." + std::string(m_textType);
    AddParameterField(path, type);
}

void PtreeWR::WriteCoordX(std::string coordX, std::string idBlock)
{
    std::string path = std::string(m_textBlock) + "." + idBlock + "." + std::string(m_textCoordX);
    AddParameterField(path, coordX);
}

void PtreeWR::WriteCoordY(std::string coordY, std::string idBlock)
{
    std::string path = std::string(m_textBlock) + "." + idBlock + "." + std::string(m_textCoordY);
    AddParameterField(path, coordY);
}

void PtreeWR::WriteParametres(QList<std::pair<QString, QVariant> > listSetting, std::string idBlock)
{
    std::string path = std::string(m_textBlock) + "." + idBlock + "." + std::string(m_textParametres);
    foreach (auto& setting, listSetting)
    {
        std::string parametr = path + "." + setting.first.toStdString();
        AddParameterField(parametr, setting.second.toString().toStdString());
    }
}

void PtreeWR::AddParameterField(const std::string& pathValue, const std::string& val)
{
    m_treeJson.put(pathValue, val);
}

std::string PtreeWR::ReadParameterValue(const std::string& pathValue)
{
    std::string value = m_treeJson.get<std::string>(pathValue, std::string());
    return value;
}

void PtreeWR::Clear()
{
    m_treeJson.clear();
}

void PtreeWR::AddBlock(BlockStruct& blockStruct)
{
    std::string id = QString::number(blockStruct.id).toStdString();
    WriteID(id);
    WriteType(blockStruct.type.toStdString(), id);
    WriteCoordX(QString::number(blockStruct.coordX).toStdString(), id);
    WriteCoordY(QString::number(blockStruct.coordY).toStdString(), id);
    WriteParametres(blockStruct.listSetting, id);
}

QMap<size_t, BlockStruct> PtreeWR::GetBlocks() const
{
    QMap<size_t, BlockStruct> blocksList;
    //создаем линии
    auto blocks = m_treeJson.get_child_optional(std::string(m_textBlock) + ".");
    if(blocks)
    {
        for (auto& el : *blocks)
        {
            BlockStruct blockStruct;
            blockStruct.id =  QString::fromStdString(el.second.get<std::string>(m_textID)).toUInt();
            blockStruct.type = QString::fromStdString(el.second.get<std::string>(m_textType));
            blockStruct.coordX = QString::fromStdString(el.second.get<std::string>(m_textCoordX)).toInt();
            blockStruct.coordY = QString::fromStdString(el.second.get<std::string>(m_textCoordY)).toInt();
            auto parametres = m_treeJson.get_child_optional(std::string(m_textBlock) + "." + el.second.get<std::string>(m_textID) + "." + std::string(m_textParametres) + ".");

            QList<std::pair<QString, QVariant> > listParam;
            if(parametres)
            {
                for (auto& param : *parametres)
                {
                    std::string parametrPath = param.first;

                    //                    boost::property_tree::write_json(std::cerr, parametres.value());

                    listParam.push_back(std::make_pair(QString::fromStdString(parametrPath),
                                                       QString::fromStdString(param.second.get_value<std::string>())));
                }
                blockStruct.listSetting = listParam;
            }
            blocksList.insert(blockStruct.id, blockStruct);
        }
    }
    return blocksList;
}

void PtreeWR::AddLink(LinkStruct linkStruct)
{
    Ptree link;
    link.put(m_textBlockIn, linkStruct.idInBlock);
    link.put(m_textNumberPortIn,  linkStruct.idInPort);
    link.put(m_textBlockOut, linkStruct.idOutBlock);
    link.put(m_textNumberPortOut, linkStruct.idOutPort);
    m_links.push_back(std::make_pair("",link));
}

QList<LinkStruct> PtreeWR::GetLinks() const
{
    QList<LinkStruct> linkStructs;
    //создаем линии
    auto links = m_treeJson.get_child_optional(std::string(m_textLink) + ".");
    if(links)
    {
        for (auto& el : *links)
        {
            LinkStruct linkStruct;
            linkStruct.idInBlock =  QString::fromStdString(el.second.get<std::string>(m_textBlockIn)).toUInt();
            linkStruct.idOutBlock = QString::fromStdString(el.second.get<std::string>(m_textBlockOut)).toUInt();
            linkStruct.idInPort = QString::fromStdString(el.second.get<std::string>(m_textNumberPortIn)).toUInt();
            linkStruct.idOutPort = QString::fromStdString(el.second.get<std::string>(m_textNumberPortOut)).toUInt();
            linkStructs.push_back(linkStruct);
        }
    }
    return linkStructs;
}

SchemeWriter::SchemeWriter(const QString &fileName): m_fileName(fileName){}

SchemeWriter::~SchemeWriter()
{
    m_ptreeWR.WriteToFile(m_fileName.toStdString());
}

void SchemeWriter::SaveBlock(BlockStruct blockStruct)
{
    m_ptreeWR.AddBlock(blockStruct);
}

void SchemeWriter::SaveLink(LinkStruct linkStruct)
{
    m_ptreeWR.AddLink(linkStruct);
}

////////////////////

SchemeReader::SchemeReader(){}

SchemeReader::~SchemeReader(){}

bool SchemeReader::ReadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly| QFile::Text))
    {
        std::cerr << "Error: Cannot read file" << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString()) << std::endl;
        return false;
    }
    file.close();
    return m_ptreeWR.ReadFileJson(fileName.toStdString());
}

QMap<size_t, BlockStruct> SchemeReader::Blocks() const
{
    return m_ptreeWR.GetBlocks();
}

QList<LinkStruct> SchemeReader::Connects() const
{
    return m_ptreeWR.GetLinks();
}
