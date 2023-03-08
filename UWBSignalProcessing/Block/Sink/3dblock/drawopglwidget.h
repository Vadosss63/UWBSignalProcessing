#ifndef DRAWOPGLWIDGET_H
#define DRAWOPGLWIDGET_H

#include <QGLWidget>
#include <math.h>
#include <QList>
#include "arrayvertex.h"
#include "../ParserData/dataparser.h"
#include "../ParserData/header.h"

// Выполняет алгоритм отрисовки
class Draw
{
public:
    virtual ~Draw();
    virtual void DrawObject(QGLWidget* qglWidget) = 0;
    QSize SizePaintRect() const;
    void SetSizePaintRect(const QSize &sizePaintRect);
private:
    QSize m_sizePaintRect;
};

class DrawOPGLWidget: public QGLWidget
{
    Q_OBJECT

public:
    DrawOPGLWidget(int size = 512, QWidget* pwgt = 0);
    ~DrawOPGLWidget();

    QSize sizePaintRect() const;
    void setSizePaintRect(const QSize &sizePaintRect);
    void ClearBuffer();
    void AddData(const std::vector<quint8> &data);

    void setShiftStart(int shiftStart);

signals:
    void MaxShift(int val);

protected:

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
private:

    void quint8ToArrayData(const std::vector<quint8> &data);
    // записывает данные в дисплейный список
    GLuint CreateLine();
    // удаляет дисплайный список из памяти
    void DeleteDisplayList(GLuint removeNumber);
    void DeleteDisplayListFromList();
    void AddDisplayList(const std::vector<quint8> &data);

    // дисплайный список для быстрого отображения
    QList<GLuint> m_dataDisplay;
    // размер поля отрисовки
    QSize m_sizePaintRect;
    ArrayData m_dataLine;
    int m_shiftStart = 0;
    int m_shiftStop = 0;

    friend class Draw;
};

#endif // DRAWOPGLWIDGET_H
