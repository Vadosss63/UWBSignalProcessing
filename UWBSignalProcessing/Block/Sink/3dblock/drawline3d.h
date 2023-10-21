#ifndef DRAWLINE3D_H
#define DRAWLINE3D_H

#include "../ParserData/dataparser.h"
#include "../ParserData/header.h"
#include "arraydata3d.h"
#include <QGLWidget>
#include <QList>
#include <math.h>

class DrawLine3D : public QGLWidget {

public:
  DrawLine3D(int size = 512, QWidget *pwgt = 0);
  ~DrawLine3D();

  QSize sizePaintRect() const;
  void setSizePaintRect(const QSize &sizePaintRect);
  void ClearBuffer();
  void AddData(const std::vector<quint8> &data);

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void timerEvent(QTimerEvent *) override;

private:
  void quint8ToArrayData(const std::vector<quint8> &data);
  // записывает данные в дисплейный список
  GLuint CreateLine();
  // удаляет дисплайный список из памяти
  void DeleteDisplayList(GLuint removeNumber);
  void DeleteDisplayListFromList();
  void AddDisplayList(const std::vector<quint8> &data);

  // дисплайный список для быстрого отображения
  QList<GLuint> m_dataDisplay;
  // размер поля отрисовки
  QSize m_sizePaintRect;
  // Парсер данных из файла
  Parser<quint8, Header> m_parserData;
  ArrayData3D m_dataLine;

  // углы поворота для координато x и у
  GLfloat m_xRotate;
  GLfloat m_yRotate;
  // координаты мыши
  QPoint m_ptPosition;
};

#endif // DRAWLINE3D_H
