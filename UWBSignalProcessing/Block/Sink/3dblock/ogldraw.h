#ifndef OGLDRAW_H
#define OGLDRAW_H

#include <QGLWidget>
#include <math.h>

class OGLDraw: public QGLWidget
{
public:
    OGLDraw(QWidget* pwgt = 0);
    void Draw(int xOffset, int yOffset, GLenum type);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // OGLDRAW_H
