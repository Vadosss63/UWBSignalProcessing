#ifndef SCOPEWIDGETOPGL_H
#define SCOPEWIDGETOPGL_H

#include <QGLWidget>
#include <math.h>
#include <QList>
#include "arrayvertex.h"

class ScopeWidgetOPGL: public QGLWidget
{
    Q_OBJECT
public:
    ScopeWidgetOPGL(int size = 512, QWidget* pwgt = 0);
    ~ScopeWidgetOPGL();

    QSize SizePaintRect() const;
    void SetSizePaintRect(const QSize &SizePaintRect);
    void ClearBuffer();
    void AddData(const std::vector<quint8> &data, bool upadeNow = true);
    void SetShiftStart(int shiftStart);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;

private:
    void Quint8ToArrayData(const std::vector<quint8> &data);
    // записывает данные в дисплейный список
    GLuint CreateLine();
    GLuint CreateGrid();
    void DeleteDisplayList(GLuint removeNumber);

    // размер поля отрисовки
    QSize m_sizePaintRect;
    ArrayData m_dataLine;
    // номер дисплайного списка линии
    GLuint m_grid;
    // дисплайный список для быстрого отображения
    QList<GLuint> m_dataDisplay;
    int m_shiftStart = 0;
};

#endif // SCOPEWIDGETOPGL_H
