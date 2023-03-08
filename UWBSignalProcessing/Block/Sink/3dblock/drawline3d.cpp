#include <QMouseEvent>
#include "drawline3d.h"

DrawLine3D::DrawLine3D(int size, QWidget *pwgt): QGLWidget (pwgt), m_sizePaintRect(size, 800),
    m_parserData("../Data/2010.09.06_07.26.03_103.rmp"), m_dataLine(size)
{
    startTimer(500);
}

DrawLine3D::~DrawLine3D(){}

void DrawLine3D::initializeGL()
{
    // устанавливает цвет для буфера
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
}

void DrawLine3D::resizeGL(int w, int h)
{
    // уставка видового окна
    glViewport(0, 0, static_cast<GLint>(w), static_cast<GLint>(h));
    // матрица проектирования
    glMatrixMode(GL_PROJECTION);
    // текущая матрица устанавливается в единичную
    glLoadIdentity();
    // устанвка соотношения сторон
 //   glOrtho(0, m_sizePaintRect.width(), 0, m_sizePaintRect.height(), -1, 1);
    glFrustum(-1, 1, -1, 1, 0, 10);

}

void DrawLine3D::ClearBuffer()
{
    //чистка буферов изображения и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawLine3D::DeleteDisplayListFromList()
{
    if(m_dataDisplay.count() > m_sizePaintRect.height())
    {
        DeleteDisplayList(m_dataDisplay.back());
        // Удаляем номер списка из контейнера
        m_dataDisplay.pop_back();
    }
}

void DrawLine3D::AddDisplayList(const std::vector<quint8> &data)
{
    quint8ToArrayData(data);
    GLuint numbDisplay = CreateLine();
    m_dataDisplay.push_front(numbDisplay);
    m_dataDisplay.front();
}

void DrawLine3D::AddData(const std::vector<quint8> &data)
{
    // Удаляем последний дисплайный список
    DeleteDisplayListFromList();
    // Добавляем новый дисплайный список
    AddDisplayList(data);
    updateGL();
}

void DrawLine3D::paintGL()
{
    //чистка буферов изображения и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // матрица моделирования
    glMatrixMode(GL_MODELVIEW);
    // текущая матрица устанавливается в единичную
    glLoadIdentity();

    // поворот системы координат на угол
    glRotatef(m_xRotate, 1.0, 0.0, 0.0);
    glRotatef(m_yRotate, 0.0, 1.0, 0.0);

    glPushMatrix();

//    glRotatef(m_xRotate+=20, 0.0, 0.0, 1.0);
//    //    пербор всех дисплейных списков
//    for (int i = 0; i < m_dataDisplay.size(); ++i) {
//        //glCallLists - как работает
//        // сдвиг системы координат по оси Y на 1 единицу
//

//        glCallList(m_dataDisplay[i]);
//    }
    glCallList(1);

    glPopMatrix();

    glFlush();
}

void DrawLine3D::mousePressEvent(QMouseEvent *event)
{
    m_ptPosition = event->pos();
}

void DrawLine3D::mouseMoveEvent(QMouseEvent *event)
{
    // вычисление углов поворота
    m_xRotate += 180 * (GLfloat)(event->y() - m_ptPosition.y()) / height();
    m_yRotate += 180 * (GLfloat)(event->x() - m_ptPosition.x()) / width();
    // обновление изображения
    updateGL();
    // актуальная координата
    m_ptPosition = event->pos();
}

QSize DrawLine3D::sizePaintRect() const
{
    return m_sizePaintRect;
}

void DrawLine3D::setSizePaintRect(const QSize &sizePaintRect)
{
    m_sizePaintRect = sizePaintRect;
}

void DrawLine3D::timerEvent(QTimerEvent *)
{
    if(m_parserData.IsNextData())
    {
        Package<std::vector<quint8>, Header> data = m_parserData.GetData();
        AddData(data.infoBody());
    }
}

void DrawLine3D::quint8ToArrayData(const std::vector<quint8> &data)
{
    for (size_t i = 0; i < data.size(); ++i) {
        m_dataLine.SetYPoint(i, data[i]);
    }
}

GLuint DrawLine3D::CreateLine()
{
    // первый свободный номер для идентификатора списка
    GLuint n = glGenLists(1);
    // создание дисплейного списка
    glNewList(n, GL_COMPILE);
    // включение поддержки масивов данных
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glEnableClientState(GL_COLOR_ARRAY);

//    glColorPointer(3, GL_UNSIGNED_BYTE, 0, m_dataLine.MassiveColor());
//    glVertexPointer(3, GL_INT, 0, m_dataLine.MassiveVertex());
//    // выполняем отрисовку подготовленного массива
//    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_dataLine.countPoint()));

//    // выключение поддержки масивов данных
//    glDisableClientState(GL_VERTEX_ARRAY);
//    glDisableClientState(GL_COLOR_ARRAY);

//        glBegin(GL_LINES);
//            glPointSize(2);
//            qglColor(Qt::green);
//            glVertex3f(0.0, 0.0, 0.0);
//            glVertex3f(0.0, 1.0, 0.0);
//            qglColor(Qt::red);
//            glVertex3f(0.0, 0.0, 0.0);
//            glVertex3f(1.0, 0.0, 0.0);
//            qglColor(Qt::blue);
//            glVertex3f(0.0, 0.0, 0.0);
//            glVertex3f(0.0, 0.0, 1.0);
//        glEnd();


            glBegin(GL_LINE_LOOP);
                glPointSize(2);
                qglColor(Qt::green);
                glVertex3f(0.5, 0.0, 1.0);
                glVertex3f(-0.5, 0.0, 1.0);
                glVertex3f(-0.5, 0.0, -1.0);
                glVertex3f(0.5, 0.0, -1.0);


//                qglColor(Qt::red);
//                glVertex3f(0.0, 0.0, 0.0);
//                glVertex3f(1.0, 0.0, 0.0);
//                qglColor(Qt::blue);
//                glVertex3f(0.0, 0.0, 0.0);
//                glVertex3f(0.0, 0.0, 1.0);
            glEnd();

    glEndList();
    return n;
}

void DrawLine3D::DeleteDisplayList(GLuint removeNumber)
{
    glDeleteLists(removeNumber, 1);
}
