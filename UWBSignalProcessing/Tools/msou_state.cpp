#include "msou_state.h"
#include <QDebug>

StateButton::StateButton(QVector<QString> listName, QWidget *parent)
    : QWidget(parent), group(new QButtonGroup(this)),
      layout(new QHBoxLayout(this)), m_timer(new QTimer(this)),
      m_amount(listName.size()) {
  for (size_t i = 0; i < m_amount; ++i) {
    QPushButton *button = new QPushButton(listName[i]);
    button->setProperty("State", false);
    button->setProperty("stateColor", "default");
    group->addButton(button, i);
  }
  connect(group->button(m_amount - 1), SIGNAL(pressed()), this,
          SLOT(HiddenButton()));
  connect(group->button(m_amount - 1), SIGNAL(pressed()), this,
          SIGNAL(ChangeSize()));
  HiddenButton();
  setLayout(layout);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(SetDefault()));
  m_timer->start(5000);
  m_style = "QPushButton {padding: 2px; border-radius: 6px; min-width: 50px; "
            "min-height: 28px;}"
            "QPushButton[stateColor=\"green\"]"
            "{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
            "stop: 0 #00ff00, stop: 1 #009f00);}"
            "QPushButton[stateColor=\"red\"]"
            "{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
            "stop: 0 #ff0000, stop: 1 #cf0000);}";

  m_defStyle = "QPushButton[stateColor=\"default\"]"
               "{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
               "stop: 0 #9f9f9f, stop: 1 #9f9f9f);}";

  setStyleSheet(m_style + m_defStyle);
  SetDefault();
}

StateButton::~StateButton() {}

bool StateButton::SetToolTips(QStringList listTips) {
  if (static_cast<size_t>(listTips.size()) != m_amount)
    return false;

  for (size_t i = 0; i < m_amount; i++)
    group->button(i)->setToolTip(listTips[i]);

  return true;
}

void StateButton::setIndicatorState(size_t index, bool state) {
  if (index < m_amount) {

    if (state) {
      if (index == m_amount - 1) {
        bool endButtonState = true;
        for (size_t i = 0; i < m_amount - 1; ++i)
          endButtonState =
              endButtonState && group->button(i)->property("State").toBool();

        if (!endButtonState) // устанавливаем желтый цвет, если какой-то из
                             // приборов не работает
        {
          group->button(index)->setProperty("stateColor", "default");
          m_timer->start(5000);
          return;
        }
      }
      // устанавливаем зеленый цвет
      group->button(index)->setProperty("stateColor", "green");
      group->button(index)->setProperty("State", true);

    } else {
      // устанавливаем красный цвет
      group->button(index)->setProperty("stateColor", "red");
      group->button(index)->setProperty("State", false);
    }
    setStyleSheet(m_style + m_defStyle);
    m_timer->start(5000);
  }
}

void StateButton::HiddenButton() {
  bool flag = group->button(0)->isHidden();
  for (size_t i = 0; i < m_amount - 1; ++i) {
    group->button(i)->setHidden(!flag);
  }
}

void StateButton::SetDefault() {
  for (size_t i = 0; i < m_amount; i++) {
    layout->addWidget(group->button(i));
    layout->setMargin(2);
    group->button(i)->setProperty("stateColor", "default");
  }
  setStyleSheet(m_style + m_defStyle);
}

void StateButton::setDefaultStyle(const QString &value) {
  m_defStyle = "QPushButton[Color=\"default\"]"
               "{" +
               value + "}";

  setStyleSheet(m_style + m_defStyle);

  SetDefault();
}

//////////////////////////////////

MSOU_State::MSOU_State(QWidget *parent)
    : StateButton(QVector<QString>() << "СИ.01"
                                     << "СИ.02"
                                     << "ЦП.01"
                                     << "ЦП.02"
                                     << "ЦП.03"
                                     << "ПУР-Х"
                                     << "ЭМС"
                                     << "МА4.1"
                                     << "МА4.2"
                                     << "МА5.1"
                                     << "МА5.2"
                                     << "МА5.3"
                                     << "МА5.4"
                                     << "МСОУ",
                  parent) {}

MSOU_State::~MSOU_State() {}
