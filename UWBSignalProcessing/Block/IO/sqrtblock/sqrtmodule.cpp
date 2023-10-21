#include "sqrtmodule.h"

SqrtModule::SqrtModule() : IOModule(1, 1) {}

void SqrtModule::Operate() {
  InBuffer temp1;
  if (!PopFromInput(temp1))
    return;

  temp1.first.pow(2);
  temp1.second.pow(2);
  temp1.first += temp1.second;

  if (m_isSqrt) {
    temp1.first.sqrt();
    PushToOutput(std::move(temp1.first));
    //        csignal<double> temp1arg = temp1;
    //        csignal<double> temp2arg = temp2;
    //        std::transform(temp1arg.begin(),temp1arg.end(), temp2arg.begin(),
    //        temp1arg.begin(),
    //                       [](double re, double im){ return
    //                       std::atan(im/re)*180/3.14;});
    //        PushToOutput(std::move(temp1arg), 1);

  } else
    PushToOutput(std::move(temp1.first), 0);
}

void SqrtModule::SetIsSqrt(bool isSqrt) { m_isSqrt = isSqrt; }
