#include "writetargetdialogbox.h"

WriteTargetDialogBox::WriteTargetDialogBox()
{
    QGridLayout* layout = new QGridLayout;
    m_textLine = new QLineEdit;
    m_textLine->setFixedWidth(500);
    m_button = new QPushButton("Обзор...");
    QLabel* text = new QLabel("Путь к файлу:");
    layout->addWidget(text, 0, 0, 1, 2);
    layout->addWidget(m_textLine, 1, 0);
    layout->addWidget(m_button, 1, 1);
    connect(m_button, SIGNAL(clicked()), this, SLOT(OpenFile()));
    SetTopLayout(layout);
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}

std::string WriteTargetDialogBox::GetPathFile() const
{
    return m_textLine->text().toStdString();
}

QList<std::pair<QString, QVariant> > WriteTargetDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant> > listSetting;
    listSetting.append(std::make_pair("m_textLine", m_textLine->text()));
    return listSetting;
}

bool WriteTargetDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
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

void WriteTargetDialogBox::TrimmedPath(QString& path)
{
    if(!path.endsWith(".target"))
    {
        path += QString(".target");
    }
}

void WriteTargetDialogBox::OpenFile()
{
    QString path = QFileDialog::getSaveFileName(this, "Сохранение файла","",
                                                "target(*.target)");
    TrimmedPath(path);

    m_textLine->setText(path);
}
