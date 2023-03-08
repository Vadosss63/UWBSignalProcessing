#ifndef OGLPYRAMID_H
#define OGLPYRAMID_H

#include <QGLWidget>
#include <math.h>

class OGLPyramid: public QGLWidget
{
public:
    OGLPyramid(QWidget* pwgt = 0);
    void Draw(int xOffset, int yOffset, GLenum type);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    GLuint CreatePyramid(GLfloat fSize = 1.0f);

private:
    // переменная дисплейного списка
    GLuint m_nPyramid;
    // углы поворота для координато x и у
    GLfloat m_xRotate;
    GLfloat m_yRotate;
    // координаты мыши
    QPoint m_ptPosition;
};

#endif // OGLPYRAMID_H
