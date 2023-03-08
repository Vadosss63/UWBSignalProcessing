#include "scope.h"
#include <iostream>

Scope::Scope(int countChanels, QSize size, QWidget *pwgt): QGLWidget (pwgt), m_sizePaintRect(size)
{
    m_dataLine.reserve(countChanels);
    for (int i = 0; i < countChanels; ++i) {
        m_dataLine.emplace_back(size.width(), 255);
    }
    connect(this, SIGNAL(ReadyData()), SLOT(updateGL()));
}

Scope::~Scope()
{
    glDeleteLists(m_grid, 1);
}

void Scope::initializeGL()
{
    // устанавливает цвет для буфера
    qglClearColor(Qt::black);
    m_grid = CreateGrid();
//    for (int i = 0; i < m_dataLine.size(); ++i) {
//        CreateLine(i);
//    }
}

void Scope::resizeGL(int w, int h)
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

void Scope::paintGL()
{
    std::cout <<" Update" << std::endl;
    //чистка буферов изображения и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < m_dataLine.size(); ++i) {
        CreateLine(i);
    }
    glCallList(m_grid);
}

QSize Scope::SizePaintRect() const
{
    return m_sizePaintRect;
}

void Scope::SetSizePaintRect(const QSize &sizePaintRect)
{
    m_sizePaintRect = sizePaintRect;
    resize(width(), height());
    glDeleteLists(m_grid, 1);
    m_grid = CreateGrid();
    for (int i = 0; i < m_dataLine.size(); ++i) {
        m_dataLine[i].SetCountPoint(sizePaintRect.width());
    }
    updateGL();
}

void Scope::SetBackgraundColor(const QColor& color)
{
    qglClearColor(color);
}

void Scope::SetColorLine(int idLine, const QColor& colorLine)
{
    if(idLine < m_dataLine.size())
        m_dataLine[idLine].SetColor(colorLine);
}

void Scope::UpateLines()
{
    updateGL();
}

void Scope::CreateLine(int idLine)
{
    // включение поддержки масивов данных
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, m_dataLine[idLine].MassiveColor());
    glVertexPointer(2, GL_INT, 0, m_dataLine[idLine].MassiveVertex());
    // выполняем отрисовку подготовленного массива
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_dataLine[idLine].countPoint()));
    // выключение поддержки масивов данных
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

GLuint Scope::CreateGrid()
{
    int countPoint = 6;
    std::vector <int> vertex(countPoint * 2);
    std::vector <u_int8_t> color(countPoint * 3, 100);
    for (int i = 0 ,countStep = 0; i < countPoint*2; i+=4, ++countStep) {
        vertex[i] = 0;
        vertex[i + 1] += m_sizePaintRect.height() * (countStep + 1) * 0.25;
        vertex[i + 2] = m_sizePaintRect.width();
        vertex[i + 3] = vertex[i + 1];
    }

    // первый свободный номер для идентификатора списка
    GLuint n = glGenLists(1);
    // создание дисплейного списка
    glNewList(n, GL_COMPILE);
    // включение поддержки масивов данных
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, color.data());
    glVertexPointer(2, GL_INT, 0, vertex.data());
    // выполняем отрисовку подготовленного массива
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(countPoint));
    // выключение поддержки масивов данных
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glEndList();
    return n;
}
