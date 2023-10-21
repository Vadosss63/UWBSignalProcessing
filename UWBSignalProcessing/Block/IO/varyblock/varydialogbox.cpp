#include "varydialogbox.h"

VARYDialogBox::VARYDialogBox() {
  auto layout = new QGridLayout;
  m_path = new QLineEdit;
  m_path->setFixedWidth(500);
  m_openButton = new QPushButton("Обзор...");
  QLabel *text = new QLabel("Путь к файлу:");
  layout->addWidget(text, 0, 0, 1, 2);
  layout->addWidget(m_path, 1, 0);
  layout->addWidget(m_openButton, 1, 1);
  connect(m_openButton, SIGNAL(clicked()), this, SLOT(OpenFile()));
  SetTopLayout(layout);
  //фиксация размеров окна
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
}

QList<std::pair<QString, QVariant>> VARYDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("pathFile", m_path->text()));
  return listSetting;
}

bool VARYDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 1)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "pathFile")
      m_path->setText(parmSetting.second.toString());
  }
  return true;
}

const std::vector<double> &VARYDialogBox::GetVARYCoeff() const {
  return m_varyCoeff;
}

void VARYDialogBox::OpenFile() {
  m_path->setText(
      QFileDialog::getOpenFileName(this, "Открытие файла", "", "txt(*.txt)"));

  if (m_path->text().isEmpty())
    return;

  std::ifstream coefFile(m_path->text().toStdString(), std::ios_base::in);
  m_varyCoeff.resize(0);
  if (coefFile.is_open()) {
    for (double coef; coefFile >> coef;) {
      m_varyCoeff.push_back(coef);
    }
    coefFile.close();
  }
}
