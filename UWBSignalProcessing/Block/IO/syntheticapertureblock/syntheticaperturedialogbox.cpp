#include "syntheticaperturedialogbox.h"
#include <qmath.h>

SyntheticApertureDialogBox::SyntheticApertureDialogBox()
{
    m_mainLayout = new QVBoxLayout;
    SetTopLayout(m_mainLayout);

    CreateWidget(4);
}

std::vector<size_t> SyntheticApertureDialogBox::GetIndexStop()
{
    size_t size = static_cast<size_t>(m_stopIndex.size());

    std::vector<size_t> indexStop(size);
    for (size_t i = 0; i < size; ++i)
        indexStop[i] = m_stopIndex[i]->value();

    return indexStop;
}


QList<std::pair<QString, QVariant> > SyntheticApertureDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::pair<QString, QVariant>("size", m_stopIndex.size()));
    for (int i = 0; i < m_stopIndex.size(); ++i)
        listSetting.append(std::make_pair(QString("stopIndex%1").arg(i), m_stopIndex[i]->value()));

    return listSetting;
}

bool SyntheticApertureDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.empty())
        return false;

    if(listSetting.first().first == "size")
    {
        int intVAl = listSetting.first().second.toInt();
        if(intVAl > 0)
            CreateWidget(intVAl);
    }
    else
        return false;
    listSetting.removeFirst();

    for (int i = 0; i < m_stopIndex.size(); ++i)
    {

        if(listSetting[i].first == QString("stopIndex%1").arg(i)){
            m_stopIndex[i]->setValue(static_cast<int>(listSetting[i].second.toDouble()));
        }
    }

    return true;
}

void SyntheticApertureDialogBox::DeleteWidget()
{
    while(!m_stopIndex.empty())
    {
        delete m_stopIndex.last();
        m_stopIndex.removeLast();
    }
}

void SyntheticApertureDialogBox::CreateWidget(int countShift)
{
    DeleteWidget();
    if(m_mainLayout->count() == 2)
    {
        auto item =  m_mainLayout->takeAt(1);
        m_mainLayout->removeItem(item);
    }
    auto layout = new QGridLayout;
    m_mainLayout->addWidget(new QLabel("Индексы конца накопления для 2, 4, 8, 16"));
    m_mainLayout->addLayout(layout);


    int row = 0;
    for (int i = 0; i < countShift; ++i)
    {

        m_stopIndex.push_back(new QSpinBox);
        layout->addWidget(m_stopIndex.back(), row % 2, i % 2);
        m_stopIndex.back()->setRange(0, 8192);

        if( i % 2)
            row++;
    }
}
