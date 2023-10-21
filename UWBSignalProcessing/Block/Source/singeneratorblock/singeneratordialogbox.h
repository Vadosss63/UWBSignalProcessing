#ifndef SINGENERATORDIALOGBOX_H
#define SINGENERATORDIALOGBOX_H

#include "abstractdialogcommand.h"
#include "customdialwidget.h"
#include <QComboBox>

class SinGeneratorDialogBox : public AbstractDialogCommand {
  Q_OBJECT
public:
  SinGeneratorDialogBox();
  ~SinGeneratorDialogBox() override = default;

  QList<std::pair<QString, QVariant>> GetSetting() override;
  bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

  double GetFrequency() const;
  double GetAmplitude() const;
  double GetPhase() const;
  bool GetSignalSize() const;

protected slots:

  void SetMaxRangeFrequecy(int index);

private:
  void CreateFrequencyWidget();
  void CreateAmplitudeWidget();
  void CreatePhaseWidget();
  void CreateSTWidget(QGridLayout *layoutSampleTime);

  QGridLayout *m_gridLayoout = nullptr;
  CustomDialWidget *m_frequency = nullptr;
  CustomDialWidget *m_amplitude = nullptr;
  CustomDialWidget *m_phase = nullptr;
  QComboBox *m_comboBoxTS = nullptr; //Частота дискретизации (Гц)
};

#endif // SINGENERATORDIALOGBOX_H
