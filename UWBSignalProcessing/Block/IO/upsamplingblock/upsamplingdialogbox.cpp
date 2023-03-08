#include "upsamplingdialogbox.h"

UpsamplingDialogBox::UpsamplingDialogBox()
{
    QGridLayout* layout = new QGridLayout;
    CreateFreqBox(layout);

    SetTopLayout(layout);

    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}

void UpsamplingDialogBox::SetN(int index)
{
    switch (index)
    {
    case 0:
        m_sampl = 2;
        break;
    case 1:
        m_sampl = 4;
        break;
    case 2:
        m_sampl = 8;
        break;
    }
}

int UpsamplingDialogBox::GetUpsamplingValue() const
{
    return m_sampl;
}

QList<std::pair<QString, QVariant> > UpsamplingDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;

    listSetting.append(std::make_pair("m_box", m_box->currentIndex()));
    return listSetting;
}

bool UpsamplingDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 1)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "m_box")
        {
            m_box->setCurrentIndex(parmSetting.second.toInt());
        }
    }
    return true;
}

void UpsamplingDialogBox::CreateFreqBox(QGridLayout *layout)
{

    QLabel* labelTS = new QLabel("N:");
    m_box = new QComboBox;

    m_box->addItem("2");
    m_box->addItem("4");
    m_box->addItem("8");

    m_box->setCurrentIndex(1);

    layout->addWidget(labelTS, 0, 0);
    layout->addWidget(m_box, 0, 1);

    connect(m_box, SIGNAL(activated(int)),this, SLOT(SetN(int)));
}
