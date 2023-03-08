#include "writedialogbox.h"

WriteDialogBox::WriteDialogBox()
{
    CreateWidget();
}

std::string WriteDialogBox::GetPathFile() const
{
    return m_pathFile_TL->text().toStdString();
}

TypeSignal WriteDialogBox::GetTypeWrite() const
{
    return static_cast<TypeSignal>(m_typeSignal_CB->currentData().toInt());
}

QList<std::pair<QString, QVariant>> WriteDialogBox::GetSetting()
{
    QList<std::pair<QString, QVariant>> listSetting;
    listSetting.append(std::make_pair("m_textLine", m_pathFile_TL->text()));
    listSetting.append(std::make_pair("m_typeSignal_CB", m_typeSignal_CB->currentIndex()));
    return listSetting;
}

bool WriteDialogBox::SetSetting(QList<std::pair<QString, QVariant> > listSetting)
{
    if(listSetting.size() != 2)
        return false;

    foreach (auto parmSetting, listSetting)
    {
        if(parmSetting.first == "m_textLine")
            m_pathFile_TL->setText(parmSetting.second.toString());

        if(parmSetting.first == "m_typeSignal_CB")
            m_typeSignal_CB->setCurrentIndex(parmSetting.second.toInt());

    }
    return true;
}

void WriteDialogBox::TrimmedPath(QString& path)
{
    if(!path.endsWith(".bin"))
        path += QString(".bin");
}

void WriteDialogBox::OpenFile()
{
    QString path = QFileDialog::getSaveFileName(this, "Сохранение файла","",
                                                "bin(*.bin)");
    TrimmedPath(path);

    m_pathFile_TL->setText(path);
}

void WriteDialogBox::CreateWidget()
{
    QGridLayout* layout = new QGridLayout;
    m_typeSignal_CB = new QComboBox;
    m_typeSignal_CB->addItem("doube", static_cast<int>(TypeSignal::doubleType));
    m_typeSignal_CB->addItem("int_8", static_cast<int>(TypeSignal::int_8_Type));
    m_typeSignal_CB->addItem("uint_8", static_cast<int>(TypeSignal::uint_8_Type));
    m_typeSignal_CB->addItem("int_16", static_cast<int>(TypeSignal::int_16_Type));
    m_typeSignal_CB->addItem("uint_16", static_cast<int>(TypeSignal::uint_16_Type));
    m_pathFile_TL = new QLineEdit;
    m_pathFile_TL->setFixedWidth(500);
    m_openFileButton = new QPushButton("Обзор...");
    layout->addWidget(new QLabel("Путь к файлу:"), 0, 0, 1, 2);
    layout->addWidget(m_pathFile_TL, 1, 0);
    layout->addWidget(m_openFileButton, 1, 1);
    layout->addWidget(new QLabel("Тип сохраняемого сигнала:"), 2, 0);
    layout->addWidget(m_typeSignal_CB, 2, 1);

    connect(m_openFileButton, SIGNAL(clicked()), this, SLOT(OpenFile()));
    SetTopLayout(layout);
    setFixedHeight(sizeHint().height());
    setFixedWidth(sizeHint().width());
}
