#ifndef CONTROLLERCOMMANDS_H
#define CONTROLLERCOMMANDS_H

#include <map>
#include <list>

#include "interfaces.h"
#include "commandcontrol.h"
#include "ptreewr.h"
#include "networkuwb.h"


// выполняет преобразования
class PTreeController: public ControllerCommands
{

public:
    PTreeController();
    ~PTreeController() override;
    // чтение принимаемой команды
    Command *ReadCommand() override;
    // чтение данных из сокета в json
    void ReadJson();
    // Запуск Приемника Команд
    void Start() override;
    // Остановка приемника команд
    void Stop() override;
    // Добавляет команду
    void AddCommand(Command* command);

private:
    // инициализация карты команд
    void InitCommands();
    // Отправка ответа
    void SendAnswer();
    // установка параметров
    void SetParameter(const std::string& param);
    template<typename TypeData>
    void SetParametrForType(const std::string& param);
    // Выполняемая команда
    Command* m_currentCommand;

    // Карта доступных команд для пользователя
    std::map<std::string, Command*> m_commands;
    // Обработчик JSON
    UtilityUWB::PtreeWR m_ptreeWR;
    // Приемник данных
    NetworkUWB::ReciverUDP m_reciverUDP;
};

template<typename TypeData>
void PTreeController::SetParametrForType(const std::string& param)
{
    TypeData value = m_ptreeWR.ReadParameterValue<TypeData>(param);
    m_currentCommand->SetParameter(param, value);
}


#endif // CONTROLLERCOMMANDS_H
