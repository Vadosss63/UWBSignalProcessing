#ifndef PANELSCAN_H
#define PANELSCAN_H

#include <QWidget>
#include <math.h>

namespace Ui {
class PanelScan;
}

class PanelScan : public QWidget {
  Q_OBJECT
public:
  explicit PanelScan(QWidget *parent = nullptr);
  ~PanelScan();

  Ui::PanelScan *GetUi() const;

  void DumpEvent(qreal bering, qreal range);

  uint8_t GetCoefStreatch() const;
  int GetVerticalShift() const;
  int GetGain() const;
  int GetUpdateStep() const;

  bool GetUpdateScan() const;
  bool GetIsColorView() const;
  bool GetIsScanLine() const;
  bool GetIsGridOnScan() const;

  void SetCoefStreatch(uint8_t coef);
  void SetVerticalShift(int shift);
  void SetGain(int gain);
  void SetUpdateStep(int updateStep);

  void SetIsGridOnScan(bool isGridOnScan);
  void SetUpdateScan(bool isUpdateScan);
  void SetColorView(bool isColorView);
  void SetIsScanLine(bool isScanLine);

public slots:
  void SetRange(uint32_t maxVal);

private:
  Ui::PanelScan *ui;
};

#endif // PANELSCAN_H
