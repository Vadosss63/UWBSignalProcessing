#ifndef ARRAYTARGET_H
#define ARRAYTARGET_H

#include <QtGlobal>
#include <QColor>
#include <vector>
#include <QOpenGLFunctions>
#include "target.h"

// обертка для сырого массива для вершин и цвета
class ArrayTarget
{
public:
    ArrayTarget();
    ~ArrayTarget();

    void SetTargetPoint(float range, float valZ, float amp, size_t index);
    void SetSpeedColorPoint(double speed, size_t index);

    // получаем указатель на вершины
    const GLfloat* MassiveVertex() const;

    // получаем указатель на знаяения цветов
    const GLubyte* MassiveColor() const;
    // количество точек
    GLsizei CountPoint() const;

    static GLfloat getMaxX();
    static void setMaxX(const GLfloat& maxX);

    static GLfloat getMaxY();
    static void setMaxY(const GLfloat& maxY);

    static GLfloat getMaxZ();
    static void setMaxZ(const GLfloat& maxZ);
    void SetTarget(const Target& target);

    static GLfloat GetAMP(GLfloat amp);
    static GLfloat GetRange(GLfloat range);
    static GLfloat GetZval(GLfloat zVal);

private:
    // количетво точек
    GLsizei m_countPoint;
    // массив вершин
    std::vector<GLfloat> m_massiveVertex;
    // массив значения цвета
    std::vector<GLubyte> m_massiveColor;

    // Плоскость лева - права
    static GLfloat m_maxValueZ;
    // Плоскость дальности
    static GLfloat m_maxValueX;

    // нормировачное значение по y
    static GLfloat m_maxValY;
    //горизонтальное отображение
    static GLfloat m_maxX;
    //вертикальное отображения
    static GLfloat m_maxY;
    // голубина отображения
    static GLfloat m_maxZ;
};

#endif // ARRAYTARGET_H
