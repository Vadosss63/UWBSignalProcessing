#include <iostream>
#include <chrono>
#include "recievercontrolmodule.h"
#include "controllercommands.h"
#include "rs485handler.h"

int main()
{

    std::cout << "Read" << std::endl;
    RecieverControlModule module;
    // Устанавливаем контроллер команд
    module.SetControllerCommands(new PTreeController);
    HandlerDevice* handler = new RS485Handler;
    bool err = handler->Open("/dev/ttyS0");


    // устанавливаем handler приемника
    if(err){
        module.SetHandlerDevice(handler);
    }else {
        std::cout << "error connect RS" << std::endl;
        delete handler;
    }

    // запускаем модуль прослушки
    module.Start();

    while (true) /// подумать как написать прослушку
        std::this_thread::sleep_for(std::chrono::minutes(10));

    return 0;
}
