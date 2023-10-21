#ifndef OGLQUAD_H
#define OGLQUAD_H

#include <QGLWidget>

class OGLQuad : public QGLWidget {
public:
  OGLQuad(QWidget *pwgt = 0);

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
};

#endif // OGLQUAD_H
