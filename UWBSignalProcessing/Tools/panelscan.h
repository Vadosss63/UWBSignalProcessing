#ifndef PANELSCAN_H
#define PANELSCAN_H

#include <QWidget>

namespace Ui {
class PanelScan;
}

class PanelScan : public QWidget
{
    Q_OBJECT
public:
    explicit PanelScan(QWidget *parent = 0);
    ~PanelScan();
    Ui::PanelScan *getUi() const;
    float GetCoefStratch();
    float GetResolution();
public slots:
    void SetRange(uint32_t maxVal);
    void Aplay();


private:
    Ui::PanelScan *ui;
    float m_coef = 1;
};

#endif // PANELSCAN_H
