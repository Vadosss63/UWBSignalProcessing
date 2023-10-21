#include "readcomplexdialogbox.h"

ReadComplexDialogBox::ReadComplexDialogBox() { CreateWidget(); }

std::string ReadComplexDialogBox::GetPathFileSigI() const {
  return m_textPathSigI->text().toStdString();
}

std::string ReadComplexDialogBox::GetPathFileSigQ() const {
  return m_textPathSigQ->text().toStdString();
}

QList<std::pair<QString, QVariant>> ReadComplexDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_textPathSigI", m_textPathSigI->text()));
  listSetting.append(std::make_pair("m_textPathSigQ", m_textPathSigQ->text()));
  return listSetting;
}

bool ReadComplexDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 2)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "m_textPathSigI")
      m_textPathSigI->setText(parmSetting.second.toString());

    if (parmSetting.first == "m_textPathSigQ")
      m_textPathSigQ->setText(parmSetting.second.toString());
  }
  return true;
}

void ReadComplexDialogBox::OpenFile() {
  QString path =
      QFileDialog::getOpenFileName(this, "Открытие файла", "", "bin(*.bin)");

  if (path.isEmpty())
    return;

  if (sender() == m_openPathSigI)
    m_textPathSigI->setText(path);
  else
    m_textPathSigQ->setText(path);
}

void ReadComplexDialogBox::CreateWidget() {
  QVBoxLayout *mainLayout = new QVBoxLayout;
  m_textPathSigI = new QLineEdit;
  m_textPathSigQ = new QLineEdit;
  m_textPathSigI->setFixedWidth(500);
  m_textPathSigQ->setFixedWidth(500);

  QGridLayout *layoutSigI = new QGridLayout;
  m_openPathSigI = new QPushButton("Обзор...");
  layoutSigI->addWidget(new QLabel("Путь к файлу:"), 0, 0, 1, 2);
  layoutSigI->addWidget(m_textPathSigI, 1, 0);
  layoutSigI->addWidget(m_openPathSigI, 1, 1);

  QGridLayout *layoutSigQ = new QGridLayout;
  m_openPathSigQ = new QPushButton("Обзор...");
  layoutSigQ->addWidget(new QLabel("Путь к файлу:"), 0, 0, 1, 2);
  layoutSigQ->addWidget(m_textPathSigQ, 1, 0);
  layoutSigQ->addWidget(m_openPathSigQ, 1, 1);

  mainLayout->addLayout(layoutSigI);
  mainLayout->addLayout(layoutSigQ);

  connect(m_openPathSigI, SIGNAL(clicked()), this, SLOT(OpenFile()));
  connect(m_openPathSigQ, SIGNAL(clicked()), this, SLOT(OpenFile()));
  SetTopLayout(mainLayout);
}
