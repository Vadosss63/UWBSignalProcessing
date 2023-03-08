#include "tradedialogbox.h"
#include <qmath.h>

TradeDialogBox::TradeDialogBox()
{
    m_mainLayout = new QVBoxLayout;
    SetTopLayout(m_mainLayout);
    m_countShift = new QComboBox;
    m_countShift->addItem("1 плата", 1);
    m_countShift->addItem("2 платы", 2);
    m_countShift->addItem("4 платы", 4);
    m_mainLayout->addWidget(m_countShift);
    connect(m_countShift, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeCountShift(int)));
    m_countShift->setCurrentIndex(1);
}

std::vector<double> TradeDialogBox::GetShiftBoard()
{
    auto size = static_cast<size_t>(m_shiftBoards.size());

    std::vector<double> shift(size);
    for (size_t i = 0; i < size; ++i)
        shift[i] = m_shiftBoards[i]->value();

    return shift;
}

void TradeDialogBox::ChangeCountShift(int index)
{
    CreateWidget(m_countShift->itemData(index).toInt());
}

QList<std::pair<QString, QVariant> > TradeDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::pair<QString, QVariant>("m_shiftSize", m_shiftBoards.size()));
    for (int i = 0; i < m_shiftBoards.size(); ++i)
        listSetting.append(std::make_pair(QString("m_shift%1").arg(i), m_shiftBoards[i]->value()));

    return listSetting;
}

bool TradeDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.empty())
        return false;

    /// TODO проверить
    if(listSetting.first().first == "m_shiftSize"){
        int intVAl = listSetting.first().second.toInt();
        if(intVAl > 0)
            CreateWidget(intVAl);
    }
    else
        return false;
    listSetting.removeFirst();

    for (int i = 0; i < m_shiftBoards.size(); ++i) {

        if(listSetting[i].first == QString("m_shift%1").arg(i)){
            m_shiftBoards[i]->setValue(listSetting[i].second.toDouble());
        }
    }

    return true;
}

void TradeDialogBox::DeleteWidget()
{
    while(!m_shiftBoards.empty())
    {
        delete m_shiftBoards.last();
        m_shiftBoards.removeLast();
    }
}

void TradeDialogBox::CreateWidget(int countShift)
{
    DeleteWidget();
    if(m_mainLayout->count() == 2)
    {
        auto item =  m_mainLayout->takeAt(1);
        m_mainLayout->removeItem(item);
    }
    auto layout = new QGridLayout;
    m_mainLayout->addLayout(layout);

    int row = 0;
    for (int i = 0; i < countShift; ++i) {

        m_shiftBoards.push_back(new QDoubleSpinBox);
        layout->addWidget(m_shiftBoards.back(), row % 2, i % 2);
        m_shiftBoards.back()->setRange(-10000, 10000);
        m_shiftBoards.back()->setDecimals(6);

        if( i % 2)
            row++;
    }

    //    //фиксация размеров окна
    //    setFixedHeight(sizeHint().height());
    //    setFixedWidth(sizeHint().width());
}
