#ifndef D3TARGETDIALOGBOX_H
#define D3TARGETDIALOGBOX_H

#include "arrayvertex.h"
#include "dialogwindow.h"
#include "interface.h"
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWheelEvent>

class D3TargetDialogBox : public QOpenGLWidget,
                          public DialogWindow,
                          public UpdateDataTarget {
  Q_OBJECT
public:
  D3TargetDialogBox();
  ~D3TargetDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  void ShowDialog() override;
  void SendCommand() override;
  void Attach(ChangeEvent *observer) override;
  void Notification() override;

  void SendCommandAbstractModule();

  // UpdateData interface
  void AddData(const std::vector<Target> &data) override;
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
  void wheelEvent(QWheelEvent *event) override;

  GLuint CreateGrid();

private:
  void CreateLine(size_t idLine);

  std::vector<ArrayTarget> m_dataLine;
  GLfloat m_xRotate = 0;
  GLfloat m_yRotate = 0;
  GLfloat m_zShift = -2;
  GLuint m_grid;
  QPoint m_pos;
};

#endif // D3TARGETDIALOGBOX_H
