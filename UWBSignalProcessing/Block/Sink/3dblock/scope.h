#ifndef SCOPE_H
#define SCOPE_H

#include "arrayvertex.h"
#include <QGLWidget>
#include <math.h>

class Scope : public QGLWidget {
  Q_OBJECT
public:
  Scope(int countChenels, QSize size = QSize(512, 256), QWidget *pwgt = 0);
  ~Scope();

  QSize SizePaintRect() const;
  void SetSizePaintRect(const QSize &SizePaintRect);
  template <typename TypeData>
  void AddData(const std::vector<TypeData> &data, int idLine);
  void SetBackgraundColor(const QColor &color);

  void SetColorLine(int idLine, const QColor &colorLine);
  void UpateLines();

signals:
  void ReadyData();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

private:
  template <typename TypeData>
  void UintToArrayData(const std::vector<TypeData> &data, int idLine);
  // записывает данные в дисплейный список
  void CreateLine(int idLine);
  GLuint CreateGrid();
  // размер поля отрисовки
  QSize m_sizePaintRect;
  std::vector<ArrayData> m_dataLine;
  // номер дисплейного списка линии
  GLuint m_grid;
};

template <typename TypeData>
void Scope::AddData(const std::vector<TypeData> &data, int idLine) {
  UintToArrayData(data, idLine);
}

template <typename TypeData>
void Scope::UintToArrayData(const std::vector<TypeData> &data, int idLine) {
  for (size_t i = 0; i < data.size(); ++i) {
    m_dataLine[idLine].SetY(i, data[i]);
  }
}

#endif // SCOPE_H
