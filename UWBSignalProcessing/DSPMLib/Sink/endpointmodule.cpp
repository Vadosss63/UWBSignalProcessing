#include "endpointmodule.h"

EndPointModule::EndPointModule(QObject* parent) :
    SinkModule(static_cast<uint8_t>(1), parent){}

EndPointModule::~EndPointModule(){}

void EndPointModule::Operate()
{
    csignal<double> temp;
    PopFromInput(temp);
}
}
