#include "arraydata3d.h"

ArrayData3D::ArrayData3D(int countPoint): m_countPoint(countPoint),
    m_massiveVertex(countPoint * 3), m_massiveColor(countPoint * 3, 255)
{
    SetXmas();
}

ArrayData3D::~ArrayData3D(){}

void ArrayData3D::SetXmas()
{
    for (int i = 0, val = 0; i < m_countPoint * 3; i+= 3, ++val) {
        m_massiveVertex[i] = val;
    }
}

void ArrayData3D::SetYmas(int valY)
{
    for (int i = 1; i < m_countPoint * 3; i+= 3) {
        m_massiveVertex[i] = valY;
    }
}

int const* ArrayData3D::MassiveVertex() const
{
    return m_massiveVertex.data();
}

quint8 const*ArrayData3D::MassiveColor() const
{
    return m_massiveColor.data();
}

void ArrayData3D::SetColorToPoint(size_t pointX, quint8 color)
{
    if(pointX < countPoint()){
        m_massiveColor[pointX * 3] = color;
        m_massiveColor[pointX * 3 + 1] = color;
        m_massiveColor[pointX * 3 + 2] = color;
    }
}

void ArrayData3D::SetYPoint(size_t pointX, int yVal)
{
    if(pointX < countPoint())
        m_massiveVertex[3 * pointX + 1] = yVal;
}

int ArrayData3D::countPoint() const
{
    return m_countPoint;
}
