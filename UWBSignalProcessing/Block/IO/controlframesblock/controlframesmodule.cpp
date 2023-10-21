#include "controlframesmodule.h"

ControlFramesModule::ControlFramesModule() : IOModule(1, 1), m_frame(0) {}

void ControlFramesModule::Operate() {
  std::lock_guard<std::mutex> lock(operateLock);

  csignal<double> tempSig;
  if (!PopFromInput(tempSig, 0))
    return;
  std::cout << tempSig.azimuth() << std::endl;
  PushToOutput(tempSig);
  //    // инициализация фрейма
  //    if (!m_frame)
  //    {
  //        m_frame = tempSig.frame_number();
  //        PushToOutput(tempSig);
  //        return;
  //    }
  //    if(m_frame + 1 == tempSig.frame_number())
  //    {
  //        PushToOutput(tempSig);
  //    }
  //    else
  //    {
  //        unsigned short tempFrame = tempSig.frame_number();
  //        for (unsigned short i = m_frame + 1; i < tempFrame + 1 ;  ++i) {
  //            tempSig.set_service_parameters(tempSig.azimuth(), i );
  //            PushToOutput(tempSig);
  //        }
  //        m_frame = tempFrame;
  //    }
}
