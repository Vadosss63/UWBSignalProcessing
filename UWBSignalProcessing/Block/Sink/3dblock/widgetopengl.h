#ifndef WIDGETOPENGL_H
#define WIDGETOPENGL_H

#include <QGLWidget>
#include <QList>
#include <math.h>

class WidgetOpenGL : public QGLWidget {
  Q_OBJECT

public:
  WidgetOpenGL(int size = 512, QWidget *pwgt = 0);
  ~WidgetOpenGL();

  QSize sizePaintRect() const;
  void setSizePaintRect(const QSize &sizePaintRect);
  void ClearBuffer();
  void AddData(const std::vector<quint8> &data);
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
  void quint8ToArrayData(const std::vector<quint8> &data);
  // записывает данные в дисплейный список
  GLuint CreateLine();
  // удаляет дисплайный список из памяти
  void DeleteDisplayList(GLuint removeNumber);
  void DeleteDisplayListFromList();
  void AddDisplayList(const std::vector<quint8> &data);
  // размер поля отрисовки
  QSize m_sizePaintRect;
};

#endif // WIDGETOPENGL_H
