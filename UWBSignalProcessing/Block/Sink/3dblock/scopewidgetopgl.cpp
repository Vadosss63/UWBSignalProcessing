#include "scopewidgetopgl.h"
#include <QMouseEvent>

ScopeWidgetOPGL::ScopeWidgetOPGL(int size, QWidget *pwgt): QGLWidget (pwgt), m_sizePaintRect(size, 256),
    m_dataLine(size, 255)
{
}

ScopeWidgetOPGL::~ScopeWidgetOPGL()
{
    ClearBuffer();
}

void ScopeWidgetOPGL::initializeGL()
{
    // устанавливает цвет для буфера
    qglClearColor(Qt::black);
    m_grid = CreateGrid();
}

void ScopeWidgetOPGL::resizeGL(int w, int h)
{
    // матрица проектирования
    glMatrixMode(GL_PROJECTION);
    // текущая матрица устанавливается в единичную
    glLoadIdentity();

    // уставка видового окна
    glViewport(0, 0, static_cast<GLint>(w), static_cast<GLint>(h));
    // устанвка соотношения сторон
    glOrtho(0, m_sizePaintRect.width(), 0, m_sizePaintRect.height(), -1, 1);
}

void ScopeWidgetOPGL::AddData(const std::vector<quint8> &data, bool upadeNow)
{
    Quint8ToArrayData(data);
    GLuint numbDisplay = CreateLine();
    m_dataDisplay.push_front(numbDisplay);
    if(upadeNow)
        updateGL();
}

void ScopeWidgetOPGL::SetShiftStart(int shiftStart)
{
    m_shiftStart = shiftStart;
    updateGL();
}

void ScopeWidgetOPGL::paintGL()
{
    //чистка буферов изображения и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // вызов отрисовки дисплайного списка
    if(!m_dataDisplay.isEmpty())
        glCallList(m_dataDisplay[m_shiftStart]);
    // отрисовка сетки
    glCallList(m_grid);

    glFlush();
}

void ScopeWidgetOPGL::mousePressEvent(QMouseEvent *){}

void ScopeWidgetOPGL::mouseMoveEvent(QMouseEvent *){}

QSize ScopeWidgetOPGL::SizePaintRect() const
{
    return m_sizePaintRect;
}

void ScopeWidgetOPGL::SetSizePaintRect(const QSize &sizePaintRect)
{
    m_sizePaintRect = sizePaintRect;
}

void ScopeWidgetOPGL::Quint8ToArrayData(const std::vector<quint8> &data)
{
    for (size_t i = 0; i < data.size(); ++i) {
        m_dataLine.SetY(i, data[i]);
    }
}

GLuint ScopeWidgetOPGL::CreateLine()
{
    // первый свободный номер для идентификатора списка
    GLuint n = glGenLists(1);
    // создание дисплейного списка
    glNewList(n, GL_COMPILE);
    // включение поддержки масивов данных
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glColorPointer(3, GL_UNSIGNED_BYTE, 0, m_dataLine.MassiveColor());
    glVertexPointer(2, GL_INT, 0, m_dataLine.MassiveVertex());
    // выполняем отрисовку подготовленного массива
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_dataLine.countPoint()));

    // выключение поддержки масивов данных
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glEndList();
    return n;
}

GLuint ScopeWidgetOPGL::CreateGrid()
{
    /// TODO оптимизировать для сетки
    ArrayData grid(512, 100);
    // первый свободный номер для идентификатора списка
    GLuint n = glGenLists(1);
    // создание дисплейного списка
    glNewList(n, GL_COMPILE);
    // включение поддержки масивов данных
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    grid.SetYmas(63);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, grid.MassiveColor());
    glVertexPointer(2, GL_INT, 0, grid.MassiveVertex());
    // выполняем отрисовку подготовленного массива
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(grid.countPoint()));

    grid.SetYmas(127);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, grid.MassiveColor());
    glVertexPointer(2, GL_INT, 0, grid.MassiveVertex());
    // выполняем отрисовку подготовленного массива
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(grid.countPoint()));

    grid.SetYmas(191);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, grid.MassiveColor());
    glVertexPointer(2, GL_INT, 0, grid.MassiveVertex());
    // выполняем отрисовку подготовленного массива
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(grid.countPoint()));

    // выключение поддержки масивов данных
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glEndList();
    return n;
}


void ScopeWidgetOPGL::DeleteDisplayList(GLuint removeNumber)
{
    glDeleteLists(removeNumber, 1);
}


void ScopeWidgetOPGL::ClearBuffer()
{
    while (!m_dataDisplay.isEmpty()) {
        DeleteDisplayList(m_dataDisplay.back());
        // Удаляем номер списка из контейнера
        m_dataDisplay.pop_back();
    }
    //чистка буферов изображения и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


