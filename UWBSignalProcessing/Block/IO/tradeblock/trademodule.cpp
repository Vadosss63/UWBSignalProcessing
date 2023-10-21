#include "trademodule.h"

TradeModule::TradeModule() : IOModule(1, 1), m_shiftConstantComponentBoard(2) {}

void TradeModule::SetShiftBoard(
    std::vector<double> shiftConstantComponentBoard) {
  m_shiftConstantComponentBoard = std::move(shiftConstantComponentBoard);
}

void TradeModule::Operate() {
  csignal<double> temp;
  if (!PopFromInput(temp))
    return;

  size_t size = temp.size();
  size_t countShift = m_shiftConstantComponentBoard.size();

  for (size_t i = 0; i < size; ++i)
    temp[i] = temp[i] - m_shiftConstantComponentBoard[i % countShift];

  PushToOutput(std::move(temp), 0);
}
