#ifndef DRAWLINE_H
#define DRAWLINE_H

#include "../ParserData/dataparser.h"
#include "../ParserData/header.h"
#include "arrayvertex.h"
#include <QGLWidget>
#include <QList>
#include <math.h>

class DrawLine : public QGLWidget {
  Q_OBJECT

public:
  DrawLine(int size = 512, QWidget *pwgt = 0);
  ~DrawLine();

  QSize sizePaintRect() const;
  void setSizePaintRect(const QSize &sizePaintRect);
  void ClearBuffer();
  void AddData(const std::vector<quint8> &data, bool upadeNow = true);
  void AddData(const std::vector<QColor> &data, bool upadeNow = true);

  void setShiftStart(int shiftStart);

signals:
  void MaxShift(int val);

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

private:
  void QUint8ToArrayData(const std::vector<quint8> &data);
  void QColorToArrayData(const std::vector<QColor> &data);
  // записывает данные в дисплейный список
  GLuint CreateLine();
  // удаляет дисплайный список из памяти
  void DeleteDisplayList(GLuint removeNumber);
  void DeleteDisplayListFromList();
  void AddDisplayList();

  // дисплайный список для быстрого отображения
  QList<GLuint> m_dataDisplay;
  // размер поля отрисовки
  QSize m_sizePaintRect;
  ArrayData m_dataLine;
  int m_shiftStart = 0;
  int m_shiftStop = 0;
};

#endif // DRAWLINE_H
