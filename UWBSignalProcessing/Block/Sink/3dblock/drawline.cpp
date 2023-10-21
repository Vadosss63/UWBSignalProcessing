#include "drawline.h"
#include <QMouseEvent>

DrawLine::DrawLine(int size, QWidget *pwgt)
    : QGLWidget(pwgt), m_sizePaintRect(size, 800), m_dataLine(size) {}

DrawLine::~DrawLine() { ClearBuffer(); }

void DrawLine::initializeGL() {
  // устанавливает цвет для буфера
  qglClearColor(Qt::black);
}

void DrawLine::resizeGL(int w, int h) {
  // матрица проектирования
  glMatrixMode(GL_PROJECTION);
  // текущая матрица устанавливается в единичную
  glLoadIdentity();

  // уставка видового окна
  glViewport(0, 0, static_cast<GLint>(w), static_cast<GLint>(h));
  // устанвка соотношения сторон
  glOrtho(0, m_sizePaintRect.width(), m_sizePaintRect.height(), 0, -1, 1);
}

void DrawLine::ClearBuffer() {
  while (!m_dataDisplay.isEmpty()) {
    DeleteDisplayList(m_dataDisplay.back());
    // Удаляем номер списка из контейнера
    m_dataDisplay.pop_back();
  }
  //чистка буферов изображения и глубины
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawLine::DeleteDisplayListFromList() {
  if (m_dataDisplay.count() <= m_sizePaintRect.height()) {
    m_shiftStop = m_dataDisplay.count();
  }
  emit MaxShift((m_dataDisplay.count()));
}

void DrawLine::AddDisplayList() {
  GLuint numbDisplay = CreateLine();
  m_dataDisplay.push_front(numbDisplay);
}

void DrawLine::setShiftStart(int shiftStart) {
  m_shiftStart = shiftStart;
  m_shiftStop = shiftStart + m_sizePaintRect.height();
  updateGL();
}

void DrawLine::AddData(const std::vector<quint8> &data, bool upadeNow) {
  // Удаляем последний дисплайный список
  DeleteDisplayListFromList();
  // Добавляем новый дисплайный список
  QUint8ToArrayData(data);
  AddDisplayList();
  if (upadeNow)
    updateGL();
}

void DrawLine::AddData(const std::vector<QColor> &data, bool upadeNow) {
  // Удаляем последний дисплайный список
  DeleteDisplayListFromList();
  // Добавляем новый дисплайный список
  QColorToArrayData(data);
  AddDisplayList();
  if (upadeNow)
    updateGL();
}

void DrawLine::paintGL() {
  //чистка буферов изображения и глубины
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  auto i = m_dataDisplay.begin();
  auto stop = m_dataDisplay.begin() + m_shiftStop + 1;

  // пербор всех дисплейных списков
  for (i += m_shiftStart; i != stop; ++i) {
    // сдвиг системы координат по оси Y на 1 единицу
    glTranslatef(0.0, 1.0, 0.0);
    glCallList((*i));
  }
  glPopMatrix();
  glFlush();
}

void DrawLine::mousePressEvent(QMouseEvent *) {}

void DrawLine::mouseMoveEvent(QMouseEvent *) {}

QSize DrawLine::sizePaintRect() const { return m_sizePaintRect; }

void DrawLine::setSizePaintRect(const QSize &sizePaintRect) {
  m_sizePaintRect = sizePaintRect;
}

void DrawLine::QUint8ToArrayData(const std::vector<quint8> &data) {
  for (size_t i = 0; i < data.size(); ++i) {
    m_dataLine.SetColorToPoint(i, data[i]);
  }
}

void DrawLine::QColorToArrayData(const std::vector<QColor> &data) {
  for (size_t i = 0; i < data.size(); ++i) {
    m_dataLine.SetColorToPoint(i, data[i]);
  }
}

GLuint DrawLine::CreateLine() {
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

void DrawLine::DeleteDisplayList(GLuint removeNumber) {
  glDeleteLists(removeNumber, 1);
}
