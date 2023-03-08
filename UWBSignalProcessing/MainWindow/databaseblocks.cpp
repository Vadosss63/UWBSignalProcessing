#include "databaseblocks.h"

LouderBlocks::LouderBlocks(){}

LouderBlocks::~LouderBlocks(){}

QList<AbstractBlock*> LouderBlocks::LoudSource()
{
    return Loud("BlocksLib/SourceBlocks");
}

QList<AbstractBlock*> LouderBlocks::LoudIO()
{
    return Loud("BlocksLib/IOBlocks");
}

QList<AbstractBlock*> LouderBlocks::LoudSink()
{
    return Loud("BlocksLib/SinkBlocks");
}

QList<AbstractBlock*> LouderBlocks::Loud(const QString& path)
{
    QList<AbstractBlock*> blocks;
    QDir dir(QApplication::applicationDirPath());
    if(!dir.cd(path))
        return blocks;

    foreach (QString fileName, dir.entryList(QDir::Files))
    {
        QPluginLoader loader(dir.absoluteFilePath(fileName));
        BilderBlocks* bilderBlocks = qobject_cast<BilderBlocks*>(loader.instance());
        if(bilderBlocks)
            blocks.append(bilderBlocks->LoudBlock());
    }
    return blocks;
}

DatabaseBlocks* DatabaseBlocks::m_instance = nullptr;

void DatabaseBlocks::LoudBlock()
{
    m_source = m_louderBlocks.LoudSource();
    m_io = m_louderBlocks.LoudIO();
    m_sink = m_louderBlocks.LoudSink();
}

DatabaseBlocks::DatabaseBlocks()
{
    LoudBlock();
}

DatabaseBlocks *DatabaseBlocks::Instance()
{
    if(!m_instance)
        m_instance = new DatabaseBlocks;

    return m_instance;
}

void DatabaseBlocks::DeleteBlock()
{
    qDeleteAll(m_source);
    qDeleteAll(m_io);
    qDeleteAll(m_sink);
}

DatabaseBlocks::~DatabaseBlocks()
{
    DeleteBlock();
}

QList<AbstractBlock*> DatabaseBlocks::SourceBlocks() const
{
    return m_source;
}

QList<AbstractBlock*> DatabaseBlocks::IOBlocks() const
{
    return m_io;
}

QList<AbstractBlock*> DatabaseBlocks::SinkBlocks() const
{
    return m_sink;
}

AbstractBlock* DatabaseBlocks::GetBlock(const QString& type) const
{
    foreach (AbstractBlock* block, m_io) {
        if(block->GetType() == type)
            return block->Clone();
    }

    foreach (AbstractBlock* block, m_source) {
        if(block->GetType() == type)
            return block->Clone();
    }

    foreach (AbstractBlock* block, m_sink) {
        if(block->GetType() == type)
            return block->Clone();
    }

    return nullptr;
}

void DatabaseBlocks::ReloudBlock()
{
    DeleteBlock();
    LoudBlock();
}
