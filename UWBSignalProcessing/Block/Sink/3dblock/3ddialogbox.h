#ifndef D3DIALOGBOX_H
#define D3DIALOGBOX_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QWheelEvent>
#include "dialogwindow.h"
#include <arrayvertex.h>
#include <interface.h>

class D3DialogBox: public QOpenGLWidget, public DialogWindow, public UpdateData
{
    Q_OBJECT
public:

    D3DialogBox();
    ~D3DialogBox() override = default;

    QList<std::pair<QString, QVariant>> GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

    void ShowDialog() override;
    void SendCommand() override;
    void Attach(ChangeEvent* observer) override;
    void Notification() override;

    void SendCommandAbstractModule();

    // UpdateData interface
    void AddData(std::vector<double>&& data, int indexZ) override;
    // сброс данных
    void ResetData() override;
    // обновление данных
    void Update() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

    GLuint CreateGrid();

private:
    void CreateLine(size_t idLine);

    std::vector<ArrayData> m_dataLine;
    GLfloat m_xRotate = 0;
    GLfloat m_yRotate = 0;
    GLfloat m_zShift = -3;
    GLuint m_grid;
    QPoint m_pos;
};

#endif // D3DIALOGBOX_H
