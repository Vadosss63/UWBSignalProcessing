#include "readtargetdialogbox.h"

ReadTargetDialogBox::ReadTargetDialogBox()
{ 
    CreateWidget();
}

std::string ReadTargetDialogBox::GetPathFile() const
{
    return m_textPath->text().toStdString();
}

QList<std::pair<QString, QVariant>> ReadTargetDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("m_textPath", m_textPath->text()));
    return listSetting;
}

bool ReadTargetDialogBox::SetSetting(QList<std::pair<QString, QVariant>> listSetting)
{
    if(listSetting.size() != 1)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "m_textPath")
            m_textPath->setText(parmSetting.second.toString());
    }
    return true;
}

void ReadTargetDialogBox::OpenFile()
{    
    QString path = QFileDialog::getOpenFileName(this, "Открытие файла","",
                                                "target(*.target)");

    if(path.isEmpty())
        return;

    m_textPath->setText(path);
}

void ReadTargetDialogBox::CreateWidget()
{
    auto* layout = new QVBoxLayout;
    m_textPath = new QLineEdit;
    m_textPath->setFixedWidth(500);

    auto* layoutSig = new QGridLayout;
    m_openPath = new QPushButton("Обзор...");
    layoutSig->addWidget(new QLabel("Путь к файлу:"), 0, 0, 1, 2);
    layoutSig->addWidget(m_textPath, 1, 0);
    layoutSig->addWidget(m_openPath, 1, 1);

    connect(m_openPath, SIGNAL(clicked()), this, SLOT(OpenFile()));

    layout->addLayout(layoutSig);

    SetTopLayout(layout);
}
