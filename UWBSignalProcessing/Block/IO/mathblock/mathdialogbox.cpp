#include "mathdialogbox.h"

MathDialogBox::MathDialogBox() {
  CreateWidget();
  setFixedHeight(sizeHint().height());
  setFixedWidth(sizeHint().width());
}

std::string MathDialogBox::GetMathState() const {
  return m_mathState->currentText().toStdString();
}

QString MathDialogBox::GetTitleState() const {
  return m_mathState->currentData().toString();
}

QList<std::pair<QString, QVariant>> MathDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("mathState", m_mathState->currentIndex()));
  return listSetting;
}

bool MathDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 1)
    return false;

  foreach (auto parmSetting, listSetting) {
    if (parmSetting.first == "mathState")
      m_mathState->setCurrentIndex(parmSetting.second.toInt());
  }
  return true;
}

void MathDialogBox::CreateWidget() {
  m_mathState = new QComboBox;

  m_mathState->addItem("Модуль", "|X|");
  m_mathState->addItem("Экспонента", "exp(x)");
  m_mathState->addItem("Квадратный корень", "X^2");
  m_mathState->addItem("Логарифм", "ln(x)");
  m_mathState->addItem("Десятичный логарифм", "log(x)");
  m_mathState->addItem("Синус", "sin(x)");
  m_mathState->addItem("Косинус", "cos(x)");
  m_mathState->addItem("Тангенс", "tan(x)");
  m_mathState->addItem("Гиперболический синус", "sinh(x)");
  m_mathState->addItem("Гиперболический косинус", "cosh(x)");
  m_mathState->addItem("Гиперболический тангенс", "tanh(x)");
  m_mathState->addItem("Арксинус", "asin(x)");
  m_mathState->addItem("Арккосинус", "acos(x)");
  m_mathState->addItem("Арктангенс", "atan(x)");

  m_mathState->setCurrentIndex(0);

  auto layout = new QGridLayout;
  layout->addWidget(new QLabel("Выберите математический оператор:"), 0, 0);
  layout->addWidget(m_mathState, 0, 1);
  SetTopLayout(layout);
}
