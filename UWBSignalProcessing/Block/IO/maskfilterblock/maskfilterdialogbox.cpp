#include "maskfilterdialogbox.h"

MaskFilterDialogBox::MaskFilterDialogBox()
{
    auto layout = new QGridLayout;
    m_textLine = new QLineEdit;
    m_textLine->setFixedWidth(500);
    m_button = new QPushButton("Обзор...");
    QLabel* text = new QLabel("Путь к файлу:");
    layout->addWidget(text, 0, 0, 1, 2);
    layout->addWidget(m_textLine, 1, 0);
    layout->addWidget(m_button, 1, 1);
    connect(m_button, SIGNAL(clicked()), this, SLOT(OpenFile()));
    SetTopLayout(layout);//фиксация размеров окна
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}

QList<std::pair<QString, QVariant>> MaskFilterDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant>> listSetting;
    listSetting.append(std::make_pair("m_textLine", m_textLine->text()));
    return listSetting;
}

bool MaskFilterDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 1)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "m_textLine")
            m_textLine->setText(parmSetting.second.toString());

    }
    return true;
}

const std::vector<double>& MaskFilterDialogBox::GetFilterArray() const
{
    return m_filterArray;
}

void MaskFilterDialogBox::OpenFile()
{
    m_textLine->setText(QFileDialog::getOpenFileName(this, "Открытие файла","",
                                                     "txt(*.txt)"));

    if(!m_textLine)
        return;

    std::ifstream coefFile(m_textLine->text().toStdString(), std::ios_base::in);
    m_filterArray.resize(0);
    if(coefFile.is_open())
    {

        for (double coef; coefFile >> coef;)
        {
            m_filterArray.push_back(coef);
        }

        coefFile.close();
        if(m_filterArray.size() != 8192)
        {
            QMessageBox msgBox;
            msgBox.setText("Предупреждение!");
            msgBox.setInformativeText(QString("Неправильный размер массива!"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;
        }
    }
}
