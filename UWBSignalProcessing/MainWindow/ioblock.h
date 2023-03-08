#ifndef IOBLOCK_H
#define IOBLOCK_H

#include "iomodule.h"
#include "abstractblock.h"
#include "lineconnect.h"

// вспомогательный класс отрисовки
// выполняет отрисовку входных и выходных портов
class IOBlock : public AbstractBlock
{

public:
    IOBlock(const QString& lable, QGraphicsItem *parent = nullptr);
    ~IOBlock() override;

    IOBlock(const IOBlock& rhs) = delete;
    IOBlock& operator=(const IOBlock& rhs) = delete;

    // отрисовка объекта
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)  override;
    // любые изменения
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    // удаление всех портов
    void ClearPort();
    // установка размера прямоугольника в заависимости от количества портов
    void SetSizeRect(uint8_t outputs, uint8_t inputs);

    QVector<IPortBlock* > GetInPort() const override;

    QVector<OPortBlock*> GetOutPort() const override;

    QList<LinkStruct> GetLinkStruct() override;

    void SetId(size_t id) override;

protected:
    //создание портов
    void CreatePorts(QVector<IPort*> iports, QVector<OPort*> oports);

private:
    // создает входные порты
    void CreateInPortBlock(QVector<IPort*> iports, uint8_t inputs, const QRectF& rect, short stepIn);

    void CreateOutPortBlock(QVector<OPort*> oports, uint8_t outputs, const QRectF& rect, short stepOut);

    // кантейнер для хранения портов
    QVector<IPortBlock*> m_inPort;
    QVector<OPortBlock*> m_outPort;

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // IOBLOCK_H
