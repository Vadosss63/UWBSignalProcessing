#include "3dtargetdialogbox.h"

D3TargetDialogBox::D3TargetDialogBox() {
  ArrayTarget::setMaxX(2);
  ArrayTarget::setMaxY(2);
  ArrayTarget::setMaxZ(0.2f);
  showFullScreen();
}

QList<std::pair<QString, QVariant>> D3TargetDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  return listSetting;
}

bool D3TargetDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  return listSetting.size() >= 3;
}

void D3TargetDialogBox::ShowDialog() {
  if (isHidden()) {
    show();
    // showFullScreen();
  } else {
    raise();
    activateWindow();
    showNormal();
  }
}

void D3TargetDialogBox::SendCommand() {}

void D3TargetDialogBox::SendCommandAbstractModule() { Notification(); }

void D3TargetDialogBox::Attach(ChangeEvent *) {}

void D3TargetDialogBox::Notification() {}

void D3TargetDialogBox::initializeGL() {
  QOpenGLFunctions *pFunc = QOpenGLContext::currentContext()->functions();
  // устанавливает цвет для буфера
  pFunc->glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
  pFunc->glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
  m_grid = CreateGrid();
}

void D3TargetDialogBox::wheelEvent(QWheelEvent *event) {
  GLfloat shift = m_zShift;
  int numDegrees = event->angleDelta().y() / 8;
  float numTicks = static_cast<float>((numDegrees / 15)) / 4;
  if (event->angleDelta().x() == 0) {
    shift += numTicks;
    if (shift > -8.5f && shift < 1.0f)
      m_zShift = shift;
  }
  update();
}

void D3TargetDialogBox::resizeGL(int w, int h) {
  // уставка видового окна
  glViewport(0, 0, static_cast<GLint>(w), static_cast<GLint>(h));
  // матрица проектирования
  glMatrixMode(GL_PROJECTION);
  // текущая матрица устанавливается в единичную
  glLoadIdentity();
  // устанвка соотношения сторон
  // glOrtho(0, m_sizePaintRect.width(), 0, m_sizePaintRect.height(), -1, 1);
  // glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
  glFrustum(-0.5, 1.0, -0.5, 1.0, 2.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
}

void D3TargetDialogBox::paintGL() {
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

void D3TargetDialogBox::mousePressEvent(QMouseEvent *event) {
  m_pos = event->pos();
}

void D3TargetDialogBox::mouseMoveEvent(QMouseEvent *event) {
  m_xRotate += 180 * static_cast<GLfloat>(event->y() - m_pos.y()) / height();
  m_yRotate += 180 * static_cast<GLfloat>(event->x() - m_pos.x()) / height();
  // updateGL
  update();
  m_pos = event->pos();
}

void D3TargetDialogBox::CreateLine(size_t idLine) {
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

GLuint D3TargetDialogBox::CreateGrid() {
  GLuint n = glGenLists(1);
  glNewList(n, GL_COMPILE);

  glBegin(GL_QUADS);
  //ось Z
  glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
  glVertex3f(0, 0, -ArrayTarget::getMaxZ());
  glVertex3f(0, 0, ArrayTarget::getMaxZ());
  glVertex3f(ArrayTarget::getMaxX(), 0, ArrayTarget::getMaxZ());
  glVertex3f(ArrayTarget::getMaxX(), 0, -ArrayTarget::getMaxZ());
  glEnd();
  glBegin(GL_LINES);
  //ось Y
  glColor4f(0.6f, 0.6f, 0.6f, 0.6f);
  glVertex3f(0, 0, 0);
  glVertex3f(0, ArrayTarget::getMaxY(), 0);

  //ось X
  glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
  glVertex3f(0, 0, 0);
  glVertex3f(ArrayTarget::getMaxX(), 0, 0);
  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

  for (int i = 0; i <= 150; i += 10) {
    glVertex3f(ArrayTarget::GetRange(i), 0, ArrayTarget::getMaxZ());
    glVertex3f(ArrayTarget::GetRange(i), 0, ArrayTarget::getMaxZ() + 0.05f);
  }

  for (int i = -15; i <= 15; i += 5) {
    glVertex3f(0, 0, ArrayTarget::GetZval(i));
    glVertex3f(-0.05f, 0, ArrayTarget::GetZval(i));
  }

  glEnd();

  glEndList();
  return n;
}

void D3TargetDialogBox::AddData(const std::vector<Target> &data) {
  if (data.empty())
    return;
  m_dataLine.resize(data.size());
  for (size_t i = 0; i < data.size(); ++i) {
    m_dataLine[i].SetTarget(data[i]);
  }
}

void D3TargetDialogBox::ResetData() {}

void D3TargetDialogBox::Update() { update(); }
