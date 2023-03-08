#include "customdialwidget.h"

CustomDialWidget::CustomDialWidget(const QString& title, QWidget *parent) :
    QWidget(parent)
{
    auto* layout = new QGridLayout;
    m_LCD = new QLCDNumber;
    m_Dial = new QDial;
    m_Dial->setNotchesVisible(true);
    m_incButton = new QPushButton("+");
    m_incButton->setFixedWidth(40);
    m_incButton->setFixedHeight(40);
    m_decButton = new QPushButton("-");
    m_decButton->setFixedWidth(40);
    m_decButton->setFixedHeight(40);
    layout->addWidget(m_Dial, 0, 0, 2, 1);
    layout->addWidget(m_LCD, 0, 1, 2, 1);
    layout->addWidget(m_incButton, 0, 2, 1, 1);
    layout->addWidget(m_decButton, 1, 2, 1, 1);

    connect(m_Dial, SIGNAL(valueChanged(int)), m_LCD, SLOT(display(int)));
    connect(m_incButton, SIGNAL(clicked()), this, SLOT(Inc()));
    connect(m_decButton, SIGNAL(clicked()), this, SLOT(Dec()));

    connect(m_Dial,SIGNAL(valueChanged(int)), this, SIGNAL(ChangeVal()) );
    connect(m_Dial,SIGNAL(valueChanged(int)), this, SIGNAL(ChangeVal(int)));

    m_GBFrequency = new QGroupBox(title);
    m_GBFrequency->setLayout(layout);
    auto* layoutMain = new QGridLayout;
    layoutMain->addWidget(m_GBFrequency);
    setLayout(layoutMain);
}

void CustomDialWidget::Inc()
{
    if(m_Dial->value() != maxVal())
        m_Dial->setValue(m_Dial->value() + 1);
}

void CustomDialWidget::Dec()
{
    if(m_Dial->value() != minVal())
        m_Dial->setValue(m_Dial->value() - 1);
}

int CustomDialWidget::minVal() const
{
    return m_minVal;
}

void CustomDialWidget::setMinVal(int minVal)
{
    m_Dial->setMinimum(minVal);
    m_minVal = minVal;
}

double CustomDialWidget::getDisplayVal()
{
    return m_LCD->value();
}

int CustomDialWidget::getDisplayIntVal()
{
    return m_LCD->intValue();
}

void CustomDialWidget::setValue(int Val)
{
    m_Dial->setValue(Val);
}

int CustomDialWidget::maxVal() const
{
    return m_maxVal;
}

void CustomDialWidget::setMaxVal(int maxVal)
{
    m_Dial->setMaximum(maxVal);
    m_maxVal = maxVal;
}
