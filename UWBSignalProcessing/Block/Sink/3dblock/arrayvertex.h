#ifndef ARRAYVERTEX_H
#define ARRAYVERTEX_H

#include <QColor>
#include <QOpenGLFunctions>
#include <QtGlobal>
#include <vector>

// обертка для сырого массива для вершин и цвета
class ArrayData {
public:
  ArrayData(size_t CountPoint);
  ~ArrayData();
  // устанавливает данные по х от 0 до countPoint
  void SetXmas();
  void SetZmas();

  void SetY(size_t valX, float valY, size_t valZ);
  // получаем указатель на вершины
  const GLfloat *MassiveVertex() const;

  // получаем указатель на знаяения цветов
  const GLubyte *MassiveColor() const;
  // количество точек
  GLsizei CountPoint() const;
  void SetCountPoint(size_t count);

  static GLfloat getMaxX();
  static void setMaxX(const GLfloat &maxX);

  static GLfloat getMaxY();
  static void setMaxY(const GLfloat &maxY);

  static GLfloat getMaxZ();
  static void setMaxZ(const GLfloat &maxZ);

private:
  // количетво точек
  GLsizei m_countPoint;
  // массив вершин
  std::vector<GLfloat> m_massiveVertex;
  // массив значения цвета
  std::vector<GLubyte> m_massiveColor;
  static std::vector<GLfloat> m_zVal;
  // количество линий по z
  static GLfloat m_countZ;
  // нормировачное значение по y
  static GLfloat m_maxValY;
  //горизонтальное отображение
  static GLfloat m_maxX;
  //вертикальное отображения
  static GLfloat m_maxY;
  // голубина отображения
  static GLfloat m_maxZ;
};

#endif // ARRAYVERTEX_H
