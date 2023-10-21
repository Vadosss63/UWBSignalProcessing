#include "arithmeticmodule.h"

ArithmeticModule::ArithmeticModule() : IOModule(2, 1) {
  Init();
  SetMathValue("Сумма");
}

ArithmeticModule::~ArithmeticModule() {
  for (auto &state : m_mapState)
    delete state.second;

  m_mapState.clear();
}

void ArithmeticModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);
  csignal<double> sigA;

  if (!PopFromInput(sigA, 0))
    return;

  csignal<double> sigB;

  if (!PopFromInput(sigB, 1))
    return;

  m_mathVal->execute(sigA, sigB);

  PushToOutput(std::move(sigA));
}

void ArithmeticModule::SetMathValue(const std::string &math_value) {
  m_mathVal = m_mapState[math_value];
}

void ArithmeticModule::Init() {
  m_mapState["Сумма"] = new SumState();
  m_mapState["Разность"] = new DiffState();
  m_mapState["Произведение"] = new MultiState();
  m_mapState["Деление"] = new DivState();
}
