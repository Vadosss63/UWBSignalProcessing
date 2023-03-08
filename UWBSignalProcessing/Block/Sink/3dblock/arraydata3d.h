#ifndef ARRAYDATA3D_H
#define ARRAYDATA3D_H

#include <QtGlobal>
#include <vector>


// обертка для сырого массива для вершин и цвета
class ArrayData3D
{
public:
    ArrayData3D(int countPoint);
    ~ArrayData3D();
    // устанавливает данные по х от 0 до countPoint
    void SetXmas();
    void SetYmas(int valY);
    // получаем указатель на вершины
    int const* MassiveVertex() const;
    // получаем указатель на знаяения цветов
    quint8 const* MassiveColor() const;
    // устанавливает значения цвета
    void SetColorToPoint(size_t pointX, quint8 color);
    // устанавливает значения высоты
    void SetYPoint(size_t pointX, int yVal);
    // количество точек
    int countPoint() const;

private:
    // количетво точек
    int m_countPoint;
    // массив вершин
    std::vector<int> m_massiveVertex;
    // массив значения цвета
    std::vector<quint8> m_massiveColor;


};


#endif // ARRAYDATA3D_H
