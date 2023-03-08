#ifndef DATABASEBLOCKS_H
#define DATABASEBLOCKS_H

#include <QPluginLoader>
#include <QString>
#include <QApplication>
#include "abstractblock.h"
#include "bilderblocks.h"

#ifndef TEST_FIENDS
#define TEST_FIENDS
#endif

class LouderBlocks
{
    TEST_FIENDS
public:

    LouderBlocks();
    ~LouderBlocks();
    QList<AbstractBlock*> LoudSource();
    QList<AbstractBlock*> LoudIO();
    QList<AbstractBlock*> LoudSink();

private:
    // выполняет загрузку блоков из библиотеки
    QList<AbstractBlock*> Loud(const QString& path);
};

class DatabaseBlocks
{
    TEST_FIENDS
public:
    // метод получения единственного экземпляра класса
    static DatabaseBlocks* Instance();
    ~DatabaseBlocks();
    // Загрузка источников
    QList<AbstractBlock*> SourceBlocks() const;
    // Загрузка модулей
    QList<AbstractBlock*> IOBlocks() const;
    // Загрузка приемников
    QList<AbstractBlock*> SinkBlocks() const;
    // получает копию блока по типу
    // если блок отсутствует то возвращает nullptr
    AbstractBlock* GetBlock(const QString& type) const;

    void ReloudBlock();

private:
    DatabaseBlocks();

    LouderBlocks m_louderBlocks;
    QList<AbstractBlock*> m_source;
    QList<AbstractBlock*> m_io;
    QList<AbstractBlock*> m_sink;

    static DatabaseBlocks* m_instance;
    void DeleteBlock();
    void LoudBlock();
};

#endif // DATABASEBLOCKS_H
