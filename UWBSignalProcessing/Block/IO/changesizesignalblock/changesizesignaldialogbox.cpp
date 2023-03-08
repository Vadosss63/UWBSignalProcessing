#include "changesizesignaldialogbox.h"

ChangeSizeSignalDialogBox::ChangeSizeSignalDialogBox()
{
    CreateWidget();
}

size_t ChangeSizeSignalDialogBox::GetStartIndex()
{
    return static_cast<size_t>(m_start->value());
}

size_t ChangeSizeSignalDialogBox::GetSiseSig()
{
    return static_cast<size_t>(m_size->value());
}

QList<std::pair<QString, QVariant> > ChangeSizeSignalDialogBox::GetSetting()
{    
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("m_start", m_start->value()));
    listSetting.append(std::make_pair("m_size", m_size->value()));

    return listSetting;
}

bool ChangeSizeSignalDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 2)
        return false;

    for(const auto& pair : listSetting) {
        if(pair.first == "m_start")
            m_start->setValue(pair.second.toInt());

        if(pair.first == "m_size")
            m_size->setValue(pair.second.toInt());
    }

    return true;
}

void ChangeSizeSignalDialogBox::CreateWidget()
{
    auto* mainLayout = new QHBoxLayout;
    m_start = new QSpinBox;
    m_start->setRange(0, 10000);
    m_size = new QSpinBox;
    m_size->setRange(0, 10000);

    mainLayout->addWidget(new QLabel("Start индекс - "));
    mainLayout->addWidget(m_start);
    mainLayout->addWidget(new QLabel("Size сигнала - "));
    mainLayout->addWidget(m_size);
    SetTopLayout(mainLayout);
}
