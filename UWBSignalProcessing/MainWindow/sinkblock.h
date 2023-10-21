#ifndef SINKBLOCK_H
#define SINKBLOCK_H

#include "abstractblock.h"
#include "lineconnect.h"
#include "sinkmodule.h"

// вспомогательный класс отрисовки
// выполняет отрисовку только входных портов
class SinkBlock : public AbstractBlock {

public:
  SinkBlock(const QString &lable, QGraphicsItem *parent = nullptr);
  ~SinkBlock() override;
  SinkBlock(const SinkBlock &rhs) = delete;
  SinkBlock &operator=(const SinkBlock &rhs) = delete;

  // отрисовка объекта
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *) override;
  // любые изменения
  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) override;

  QVector<IPortBlock *> GetInPort() const override;

  QVector<OPortBlock *> GetOutPort() const override;

  void SetId(size_t id) override;

protected:
  //создание портов
  void CreatePorts(QVector<IPort *> ports);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
  // входные порты
  QVector<IPortBlock *> m_port;
};

#endif // SINKBLOCK_H
