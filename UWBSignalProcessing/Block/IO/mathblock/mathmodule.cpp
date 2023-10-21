#include "mathmodule.h"

MathModule::MathModule() : IOModule(1, 1) {
  Init();
  SetMathValue("Модуль");
}

MathModule::~MathModule() {
  for (const auto &state : m_mapState)
    delete state.second;

  m_mapState.clear();
}

void MathModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  csignal<double> temp;

  if (!PopFromInput(temp))
    return;

  m_mathVal->execute(temp);

  PushToOutput(std::move(temp));
}

void MathModule::SetMathValue(const std::string &math_value) {
  m_mathVal = m_mapState[math_value];
}

void MathModule::Init() {
  m_mapState["Модуль"] = new StateAbs();
  m_mapState["Экспонента"] = new StateExp();
  m_mapState["Квадратный корень"] = new StateSqrt();
  m_mapState["Логарифм"] = new StateLog();
  m_mapState["Десятичный логарифм"] = new StateLog10();
  m_mapState["Синус"] = new StateSin();
  m_mapState["Косинус"] = new StateCos();
  m_mapState["Тангенс"] = new StateTan();
  m_mapState["Гиперболический синус"] = new StateSinh();
  m_mapState["Гиперболический косинус"] = new StateCosh();
  m_mapState["Гиперболический тангенс"] = new StateTanh();
  m_mapState["Арксинус"] = new StateAsin();
  m_mapState["Арккосинус"] = new StateAcos();
  m_mapState["Арктангенс"] = new StateAtan();
}
