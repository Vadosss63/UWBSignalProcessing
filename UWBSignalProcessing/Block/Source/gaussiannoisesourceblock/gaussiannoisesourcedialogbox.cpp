#include "gaussiannoisesourcedialogbox.h"

GaussianNoiseSourceDialogBox::GaussianNoiseSourceDialogBox()
{
    CreateWidget();
    //фиксация размеров окна
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}

void GaussianNoiseSourceDialogBox::CreateWidget()
{
    m_customDialWidget = new CustomDialWidget("СКО");
    m_customDialWidget->setMaxVal(200);
    m_customDialWidget->setMinVal(1);
    QGridLayout* layoutSampleTime = new QGridLayout;
    QLabel* labelTS = new QLabel("Частота дискретизации:");
    m_comboBoxTS = new QComboBox;
    m_comboBoxTS->addItem("0.5 ГГц");
    m_comboBoxTS->addItem("2 ГГц");
    layoutSampleTime->addWidget(labelTS, 0, 0);
    layoutSampleTime->addWidget(m_comboBoxTS, 0, 1);
    QGridLayout* gl = new QGridLayout;
    gl->addLayout(layoutSampleTime, 0, 0);
    gl->addWidget(m_customDialWidget);
    SetTopLayout(gl);
}

size_t GaussianNoiseSourceDialogBox::GetSignalSize()
{
    size_t size = m_comboBoxTS->currentIndex() ? 2048 * 4 : 2048;
    return size;
}

double GaussianNoiseSourceDialogBox::GetDeviation()
{
    double deviation = m_customDialWidget->getDisplayIntVal();
    return deviation;
}

QList<std::pair<QString, QVariant> > GaussianNoiseSourceDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("m_comboBoxTS",m_comboBoxTS->currentIndex()));
    listSetting.append(std::make_pair("m_customDialWidget", m_customDialWidget->getDisplayIntVal()));

    return listSetting;
}

bool GaussianNoiseSourceDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 2)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "m_comboBoxTS")
            m_comboBoxTS->setCurrentIndex(parmSetting.second.toInt());

        if(parmSetting.first == "m_customDialWidget")
            m_customDialWidget->setValue(parmSetting.second.toInt());

    }
    return true;

}
