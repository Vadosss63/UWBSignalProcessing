#include "oglpyramid.h"
#include <QMouseEvent>

OGLPyramid::OGLPyramid(QWidget *pwgt)
    : QGLWidget(pwgt), m_xRotate(0), m_yRotate(0) {}

void OGLPyramid::initializeGL() {
  // устанавливает цвет для буфера
  qglClearColor(Qt::black);
  // проверка глубины фрагментов
  glEnable(GL_DEPTH_TEST);
  // выключение сглаживания
  glShadeModel(GL_FLAT);
  // создает дисплейный список и возращает его номер
  m_nPyramid = CreatePyramid(1.2f);
}

void OGLPyramid::resizeGL(int w, int h) {
  // уставка видового окна
  glViewport(0, 0, static_cast<GLint>(w), static_cast<GLint>(h));
  // матрица проектирования
  glMatrixMode(GL_PROJECTION);
  // текущая матрица устанавливается в единичную
  glLoadIdentity();

  // Задание пирамиды видимости
  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10);
}

void OGLPyramid::paintGL() {
  //чистка буферов изображения и глубины
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // матрица моделирования
  glMatrixMode(GL_MODELVIEW);
  // текущая матрица устанавливается в единичную
  glLoadIdentity();
  // сдвиг системы координат по оси Z на 3 единицы
  glTranslatef(0.0, 0.0, -3.0);
  // поворот системы координат на угол
  glRotatef(m_xRotate, 1.0, 0.0, 0.0);
  glRotatef(m_yRotate, 0.0, 1.0, 0.0);
  // передача номера дисплейного списка и отображение его
  glCallList(m_nPyramid);
}

void OGLPyramid::mousePressEvent(QMouseEvent *event) {
  m_ptPosition = event->pos();
}

void OGLPyramid::mouseMoveEvent(QMouseEvent *event) {
  // вычисление углов поворота
  m_xRotate += 180 * (GLfloat)(event->y() - m_ptPosition.y()) / height();
  m_yRotate += 180 * (GLfloat)(event->x() - m_ptPosition.x()) / width();
  // обновление изображения
  updateGL();
  // актуальная координата
  m_ptPosition = event->pos();
}

GLuint OGLPyramid::CreatePyramid(GLfloat fSize) {
  // первый свободный номер для идентификатора списка
  GLuint n = glGenLists(1);
  // создание дисплейного списка
  glNewList(n, GL_COMPILE);
  // треугольник с общей вершиной
  glBegin(GL_TRIANGLE_FAN);
  qglColor(Qt::green);
  glVertex3f(0.0, fSize, 0.0);
  glVertex3f(-fSize, -fSize, fSize);
  glVertex3f(fSize, -fSize, fSize);
  qglColor(Qt::yellow);
  glVertex3f(fSize, -fSize, -fSize);
  qglColor(Qt::blue);
  glVertex3f(-fSize, -fSize, -fSize);
  qglColor(Qt::white);
  glVertex3f(-fSize, -fSize, fSize);
  glEnd();

  glBegin(GL_QUADS);
  qglColor(Qt::red);
  glVertex3f(-fSize, -fSize, fSize);
  glVertex3f(fSize, -fSize, fSize);
  glVertex3f(fSize, -fSize, -fSize);
  glVertex3f(-fSize, -fSize, -fSize);
  glEnd();

  glEndList();
  return n;
}
