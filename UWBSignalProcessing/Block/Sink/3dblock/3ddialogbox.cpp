#include "3ddialogbox.h"

D3DialogBox::D3DialogBox(): m_dataLine(256, 512)
{
    ArrayData::setMaxX(2);
    ArrayData::setMaxY(2);
    ArrayData::setMaxZ(1);
    showFullScreen();
}

QList<std::pair<QString, QVariant>> D3DialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant>> listSetting;
    return listSetting;
}

bool D3DialogBox::SetSetting(QList<std::pair<QString, QVariant>> listSetting)
{
    if(listSetting.size() < 3)
        return false;

    return true;
}

void D3DialogBox::ShowDialog()
{
    if(isHidden())
    {
        show();
        //showFullScreen();
    }
    else
    {
        raise();
        activateWindow();
        showNormal();
    }
}

void D3DialogBox::SendCommand(){}

void D3DialogBox::SendCommandAbstractModule()
{
    Notification();
}

void D3DialogBox::Attach(ChangeEvent* observer){}

void D3DialogBox::Notification(){}

void D3DialogBox::initializeGL()
{
    QOpenGLFunctions* pFunc = QOpenGLContext::currentContext()->functions();
    // устанавливает цвет для буфера
    pFunc->glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    pFunc->glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    m_grid = CreateGrid();
}


void D3DialogBox::wheelEvent(QWheelEvent *event)
{
    GLfloat shift = m_zShift;
    int numDegrees = event->delta() / 8;
    float numTicks = static_cast<float>((numDegrees / 15))/2;
    if(event->orientation() == Qt::Vertical)
    {
        shift += numTicks;
        if(shift > -8.5f && shift < -1.0f)
            m_zShift = shift;
    }
    update();
}

void D3DialogBox::resizeGL(int w, int h)
{
    // уставка видового окна
    glViewport(0, 0, static_cast<GLint>(w), static_cast<GLint>(h));
    // матрица проектирования
    glMatrixMode(GL_PROJECTION);
    // текущая матрица устанавливается в единичную
    glLoadIdentity();
    // устанвка соотношения сторон
    // glOrtho(0, m_sizePaintRect.width(), 0, m_sizePaintRect.height(), -1, 1);
    // glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
    glFrustum(-0.5, 1.0, -0.5, 1.0, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void D3DialogBox::paintGL()
{
    //чистка буферов изображения и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    // Присваевает матрице маделирования единичную функцию
    glLoadIdentity();
    //сдвиг оси z
    glTranslatef(0.0, 0.0, m_zShift);
    // Поворот по x
    glRotatef(m_xRotate, 1.0, 0.0, 0.0);
    // Поворот по y
    glRotatef(m_yRotate, 0.0, 1.0, 0.0);
    glCallList(m_grid);
    for (size_t i = 0; i < m_dataLine.size(); ++i)
        CreateLine(i);
}

void D3DialogBox::mousePressEvent(QMouseEvent* event)
{
    m_pos = event->pos();
}

void D3DialogBox::mouseMoveEvent(QMouseEvent* event)
{
    m_xRotate += 180 * static_cast<GLfloat>(event->y() - m_pos.y()) / height();
    m_yRotate += 180 * static_cast<GLfloat>(event->x() - m_pos.x()) / height();
    // updateGL
    update();
    m_pos = event->pos();
}

void D3DialogBox::CreateLine(size_t idLine)
{
    // включение поддержки масивов данных
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, m_dataLine[idLine].MassiveColor());
    glVertexPointer(3, GL_FLOAT, 0, m_dataLine[idLine].MassiveVertex());
    // выполняем отрисовку подготовленного массива
    glDrawArrays(GL_LINE_STRIP, 0, m_dataLine[idLine].CountPoint());
    // выключение поддержки масивов данных
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

const GLubyte* ArrayData::MassiveColor() const
{
    return m_massiveColor.data();
}

GLuint D3DialogBox::CreateGrid()
{
    GLuint n = glGenLists(1);
    glNewList(n, GL_COMPILE);

    glBegin(GL_QUADS);
    //ось Z
    glColor4f(0.3f, 1.0f, 0.3f, 1.0f);
    glVertex3f(0, 0, -ArrayData::getMaxZ());
    glVertex3f(0, 0, ArrayData::getMaxZ());
    glVertex3f(ArrayData::getMaxX(), 0, ArrayData::getMaxZ());
    glVertex3f(ArrayData::getMaxX(), 0, -ArrayData::getMaxZ());
    glEnd();
    glBegin(GL_LINES);
    //ось Y
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, ArrayData::getMaxY(), 0);

    //ось X
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(ArrayData::getMaxX(), 0, 0);
    glEnd();


    glEndList();
    return n;
}


void D3DialogBox::AddData(std::vector<double>&& data, int indexZ)
{
    for (size_t i = 0; i < data.size(); ++i)
        m_dataLine[indexZ].SetY(i, static_cast<float>(data[i]), indexZ);
}

void D3DialogBox::ResetData()
{
}

void D3DialogBox::Update()
{
    update();
}
