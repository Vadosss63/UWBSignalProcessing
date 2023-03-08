#include "arrayvertex.h"

GLfloat ArrayData::m_maxX = 2;
GLfloat ArrayData::m_maxY = 2;
GLfloat ArrayData::m_maxZ = 1;
GLfloat ArrayData::m_countZ = 256;
GLfloat ArrayData::m_maxValY = 2048;
std::vector<GLfloat> ArrayData::m_zVal = std::vector<GLfloat>();

ArrayData::ArrayData(size_t countPoint):
    m_countPoint(static_cast<GLsizei>(countPoint)), m_massiveVertex(countPoint * 3),
    m_massiveColor(countPoint * 3, 128)
{
    SetXmas();
    SetZmas();
}

ArrayData::~ArrayData(){}

void ArrayData::SetXmas()
{
    GLfloat val = 0;
    GLfloat shift = getMaxX() / (CountPoint() - 1);
    for (size_t i = 0; i < m_massiveVertex.size(); i+= 3) {
        m_massiveVertex[i] = val;
        val += shift;
    }
}

void ArrayData::SetZmas()
{
    if(!m_zVal.empty()) return;
    m_zVal.resize(static_cast<size_t>(m_countZ));

    GLfloat val = -1;
    GLfloat shift = 2 * getMaxZ() / m_countZ;
    for (size_t i = 0; i < m_countZ; ++i) {
        m_zVal[i] = val;
        val += shift;
    }
}

void ArrayData::SetY(size_t valX, float valY, size_t valZ)
{
    if(valY > m_maxValY)
        valY = m_maxValY;

    GLfloat valYf = (valY * m_maxY) / m_maxValY;

    m_massiveVertex[(valX * 3) + 1] = valYf;
    m_massiveVertex[(valX * 3) + 2] = m_zVal[valZ];
}

const GLfloat* ArrayData::MassiveVertex() const
{
    return m_massiveVertex.data();
}

GLsizei ArrayData::CountPoint() const
{
    return m_countPoint;
}

void ArrayData::SetCountPoint(size_t count)
{
    m_countPoint = static_cast<GLsizei>(count);
    m_massiveVertex.resize(count * 3);
    SetXmas();
}

GLfloat ArrayData::getMaxX()
{
    return m_maxX;
}

void ArrayData::setMaxX(const GLfloat& maxX)
{
    m_maxX = maxX;
}

GLfloat ArrayData::getMaxY()
{
    return m_maxY;
}

void ArrayData::setMaxY(const GLfloat& maxY)
{
    m_maxY = maxY;
}

GLfloat ArrayData::getMaxZ()
{
    return m_maxZ;
}

void ArrayData::setMaxZ(const GLfloat& maxZ)
{
    m_maxZ = maxZ;
}
