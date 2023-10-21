#include "ogldraw.h"

OGLDraw::OGLDraw(QWidget *pwgt) : QGLWidget(pwgt) {}

void OGLDraw::Draw(int xOffset, int yOffset, GLenum type) {
  int n = 8;

  glPointSize(10);
  glBegin(type);
  glColor3f(0, 0, 0);
  for (int i = 0; i < n; ++i) {
    float fAngle = 2 * 3.14 * i / n;
    int x = (int)(50 + std::cos(fAngle) * 40 + xOffset);
    int y = (int)(50 + std::sin(fAngle) * 40 + yOffset);
    glVertex2f(x, y);
  }
  glEnd();
}

void OGLDraw::initializeGL() {
  // устанавливает цвет для буфера
  /// glClearColor(); аналог gl
  qglClearColor(Qt::white);
  // GL_SMOOTH - вкл
  glShadeModel(GL_FLAT); // отключает сглаживание
}

void OGLDraw::resizeGL(int w, int h) {
  // матрица проектирования
  glMatrixMode(GL_PROJECTION);
  // текущая матрица устанавливается в единичную
  glLoadIdentity();
  // уставка видового окна
  glViewport(0, 0, static_cast<GLint>(w), static_cast<GLint>(h));
  // устанвка соотношения сторон
  glOrtho(0, 400, 200, 0, -1, 1);
}

void OGLDraw::paintGL() {
  //чистка буферов изображения и глубины
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Draw(0, 0, GL_POINTS);
  Draw(100, 0, GL_LINES);
  Draw(200, 0, GL_LINE_STRIP);
  Draw(300, 0, GL_LINE_LOOP);

  Draw(0, 100, GL_TRIANGLE_STRIP);
  Draw(100, 100, GL_POLYGON);
  Draw(200, 100, GL_QUADS);
  Draw(300, 100, GL_TRIANGLES);
}
