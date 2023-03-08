#include "arithmeticdialogbox.h"

ArithmeticDialogBox::ArithmeticDialogBox()
{
    CreateWidget();
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}

std::string ArithmeticDialogBox::GetState() const
{
    return m_arithmeticState->currentText().toStdString();
}

QString ArithmeticDialogBox::GetTitleState() const
{
    return m_arithmeticState->currentData().toString();
}

QList<std::pair<QString, QVariant>> ArithmeticDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant>> listSetting;
    listSetting.append(std::make_pair("arithmeticState", m_arithmeticState->currentIndex()));
    return listSetting;
}

bool ArithmeticDialogBox::SetSetting(QList<std::pair<QString, QVariant>> listSetting)
{
    if(listSetting.size() != 1)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "arithmeticState")
            m_arithmeticState->setCurrentIndex(parmSetting.second.toInt());

    }
    return true;
}

void ArithmeticDialogBox::CreateWidget()
{    
    m_arithmeticState = new QComboBox;

    m_arithmeticState->addItem("Сумма", "a+b");
    m_arithmeticState->addItem("Разность", "a-b");
    m_arithmeticState->addItem("Произведение", "a*b");
    m_arithmeticState->addItem("Деление", "a/b");

    m_arithmeticState->setCurrentIndex(0);
    auto layout = new QGridLayout;
    layout->addWidget(new QLabel("Математический оператор:"), 0, 0);
    layout->addWidget(m_arithmeticState, 0, 1);
    SetTopLayout(layout);
}
