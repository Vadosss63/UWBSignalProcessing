#ifndef SHOWTARGETMODULE_H
#define SHOWTARGETMODULE_H

#include <algorithm>
#include <iostream>
#include "sinkmodule.h"
#include "intrface.h"
#include "target.h"

class ShowTargetModule: public SinkModule<std::vector<Target>>
{

public:

    ShowTargetModule();
    ~ShowTargetModule() override = default;

    // Установка интерфейса обновления класса
    void SetUpdateScane(UpdateScane* updateScane);
    void Operate() override;

private:
    UpdateScane* m_updateScane = nullptr;

};

#endif // SHOWTARGETMODULE_H
