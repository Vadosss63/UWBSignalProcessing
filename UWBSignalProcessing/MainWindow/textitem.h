#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QGraphicsRectItem>

// класса выполняет создание текстового элемента
// для отборажения на QGraphicsScene
class TextItem : public QGraphicsRectItem {

public:
  // выполняет установку размеров элемнта
  TextItem(const QString &text, const QRectF &rectF);
  ~TextItem();
  // событие отрисовки
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

private:
  // текст элемента
  QString m_text;
};

#endif // TEXTITEM_H
