#include "arrayvertex.h"

GLfloat ArrayTarget::m_maxX = 2;
GLfloat ArrayTarget::m_maxY = 2;
GLfloat ArrayTarget::m_maxZ = 1;
GLfloat ArrayTarget::m_maxValY = 2048;

GLfloat ArrayTarget::m_maxValueZ = 30;
GLfloat ArrayTarget::m_maxValueX = 150;

ArrayTarget::ArrayTarget(){}

ArrayTarget::~ArrayTarget(){}

GLfloat  ArrayTarget::GetAMP(GLfloat amp)
{
    return (amp * m_maxY) / m_maxValY;
}

GLfloat ArrayTarget::GetRange(GLfloat range)
{
    return  (range * m_maxX) / m_maxValueX;
}

GLfloat ArrayTarget::GetZval(GLfloat zVal)
{
    return (zVal * m_maxZ) * 2 / m_maxValueZ;
}

void ArrayTarget::SetTargetPoint(float range, float valZ, float amp, size_t index)
{
    amp *= static_cast<float>(10E-3);
    if(amp > m_maxValY)
        amp = m_maxValY;

    GLfloat ampf = GetAMP(amp);
    GLfloat rangef = GetRange(range);
    GLfloat zValf = GetZval(valZ);

    // Дальность
    m_massiveVertex[(index * 3)] = rangef;
    // Амплитуда
    m_massiveVertex[(index * 3) + 1] = ampf;
    // Отклонение
    m_massiveVertex[(index * 3) + 2] = zValf;
}

void ArrayTarget::SetSpeedColorPoint(double speed, size_t index)
{
    static double max = 216;
    if(speed > 0)
    {
        double coef = 1 - speed / max;

        //R
        m_massiveColor[(index * 3)] = 255 * coef;
        //G
        m_massiveColor[(index * 3) + 1] = 0;
        //B
        m_massiveColor[(index * 3) + 2] = 0;
    }else    {
        double coef = 1 + speed / max;

        //R
        m_massiveColor[(index * 3)] = 0;
        //G
        m_massiveColor[(index * 3) + 1] = 0;
        //B
        m_massiveColor[(index * 3) + 2] = 255 * coef;
    }


}

const GLfloat* ArrayTarget::MassiveVertex() const
{
    return m_massiveVertex.data();
}

const GLubyte*ArrayTarget::MassiveColor() const
{
    return m_massiveColor.data();
}

GLsizei ArrayTarget::CountPoint() const
{
    return m_countPoint;
}

GLfloat ArrayTarget::getMaxX()
{
    return m_maxX;
}

void ArrayTarget::setMaxX(const GLfloat& maxX)
{
    m_maxX = maxX;
}

GLfloat ArrayTarget::getMaxY()
{
    return m_maxY;
}

void ArrayTarget::setMaxY(const GLfloat& maxY)
{
    m_maxY = maxY;
}

GLfloat ArrayTarget::getMaxZ()
{
    return m_maxZ;
}

void ArrayTarget::setMaxZ(const GLfloat& maxZ)
{
    m_maxZ = maxZ;
}

void ArrayTarget::SetTarget(const Target& target)
{
    m_countPoint = static_cast<GLsizei>(target.Size());
    m_massiveVertex = std::vector<GLfloat>(m_countPoint * 3);
    m_massiveColor = std::vector<GLubyte>(m_countPoint * 3, 255);
    size_t index = 0;
    for(const Target::Point& point : target.GetPoints())
    {
        float x = static_cast<float>(target.GetX(point));
        float y = static_cast<float>(target.GetY(point));
        SetTargetPoint(y, x, point.amp, index);
        if(point.speed)
            SetSpeedColorPoint(target.GetToSpeedKMH(point.speed), index);
        ++index;
    }
}
