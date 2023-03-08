#include "oglquad.h"

OGLQuad::OGLQuad(QWidget *pwgt): QGLWidget (pwgt)
{
}

void OGLQuad::initializeGL()
{
    // устанавливает цвет для буфера
    /// glClearColor(); аналог gl
    qglClearColor(Qt::black);
    // GL_SMOOTH - вкл
    //glShadeModel(GL_FLAT);// отключает сглаживание

}

void OGLQuad::resizeGL(int w, int h)
{
    // матрица проектирования
    glMatrixMode(GL_PROJECTION);
    // текущая матрица устанавливается в единичную
    glLoadIdentity();
    // уставка видового окна
    glViewport(0, 0, static_cast<GLint>(w), static_cast<GLint>(h));
    // устанвка соотношения сторон
    glOrtho(0, 100, 100, 0, -1, 1);
}

void OGLQuad::paintGL()
{
    //чистка буферов изображения и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // начало отрисовки по алгоритму GL_QUADS - квадрата
    glBegin(GL_QUADS);
    // задает текущий цвет (RGB)
    glColor3f(1, 0, 0);
    // положение вершин
    glVertex2d(10, 90);

    glColor3f(0, 1, 0);
    glVertex2d(90, 90);

    glColor3f(0, 0, 1);
    glVertex2d(90, 10);

    glColor3f(1, 1, 1);
    glVertex2d(10, 10);
    glEnd();
}
