#ifndef CUSTOMDIALWIDGET_H
#define CUSTOMDIALWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QDial>
#include <QPushButton>
#include <QGroupBox>

class CustomDialWidget: public QWidget
{
    Q_OBJECT
public:
    CustomDialWidget(const QString& title, QWidget *parent = nullptr);
    ~CustomDialWidget() = default;

    int maxVal() const;
    void setMaxVal(int maxVal);

    int minVal() const;
    void setMinVal(int minVal);

    double getDisplayVal();
    int getDisplayIntVal();

    void setValue(int Val);

signals:
    void ChangeVal();
    // для стандарта
    void ChangeVal(int);

private slots:
    void Inc();
    void Dec();

private:

    int m_maxVal = 100;
    int m_minVal = 0;
    QGridLayout* m_gridLayoout = nullptr;
    QLCDNumber* m_LCD = nullptr;
    QDial* m_Dial = nullptr;
    QPushButton* m_incButton = nullptr;
    QPushButton* m_decButton = nullptr;
    QGroupBox* m_GBFrequency = nullptr;
};

#endif // CUSTOMDIALWIDGET_H
