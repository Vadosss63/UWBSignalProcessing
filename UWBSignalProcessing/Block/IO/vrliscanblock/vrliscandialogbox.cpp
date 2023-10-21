#include "vrliscandialogbox.h"
#include <QSettings>

VRLIScanDialogBox::VRLIScanDialogBox() {
  CreateWidget();
  SetTopLayout(m_gridLayout);
}

bool VRLIScanDialogBox::GetCreateVRLI() { return m_isPRLI->currentIndex() % 2; }

QList<std::pair<QString, QVariant>> VRLIScanDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("m_isPRLI", m_isPRLI->currentIndex()));

  return listSetting;
}

bool VRLIScanDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {

  if (listSetting.size() != 1)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "m_isPRLI")
      m_isPRLI->setCurrentIndex(parmSetting.second.toInt());
  }

  return true;
}

void VRLIScanDialogBox::CreateWidget() {
  m_gridLayout = new QGridLayout;
  m_isPRLI = new QComboBox;
  m_isPRLI->addItem("ПРЛИ");
  m_isPRLI->addItem("ВРЛИ");
  m_gridLayout->addWidget(m_isPRLI);
}
