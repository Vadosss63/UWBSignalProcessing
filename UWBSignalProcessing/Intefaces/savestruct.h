#ifndef SAVESTRUCT_H
#define SAVESTRUCT_H

#include <QMap>
#include <QVariant>

struct BlockStruct
{
    size_t id = 0;
    QString type;
    int coordX;
    int coordY;
    QList<std::pair<QString, QVariant>> listSetting;
};

struct LinkStruct
{
    size_t idInBlock;
    size_t idInPort;

    size_t idOutBlock;
    size_t idOutPort;
};

#endif // SAVESTRUCT_H
