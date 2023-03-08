#ifndef SOURCEBLOCK_H
#define SOURCEBLOCK_H

#include "abstractblock.h"
#include "lineconnect.h"
#include "sourcemodule.h"

// вспомогательный класс отрисовки
// выполняет отрисовку только выходных портов

class SourceBlock : public AbstractBlock
{    
public:
    SourceBlock(const QString& lable, QGraphicsItem *parent = nullptr);
    ~SourceBlock() override;
    SourceBlock(const SourceBlock& rhs) = delete;
    SourceBlock& operator=(const SourceBlock& rhs) = delete;

    // отрисовка объекта
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*) override;
    // любые изменения
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    QVector<OPortBlock*> GetOutPort() const override;

    QVector<IPortBlock*> GetInPort() const override;

    QList<LinkStruct> GetLinkStruct() override;

    void SetId(size_t id) override;

protected:
    //создание портов
    void CreatePorts(QVector<OPort*> ports);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
private:
    // очистка портов
    void ClearPorts();
    // выходные порты
    QVector<OPortBlock *> m_port;
};

#endif // SOURCEBLOCK_H
