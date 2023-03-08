#include <QtTest>
#include "gainmodule.h"

class Test_GainModule : public QObject
{
    Q_OBJECT

public:
    Test_GainModule();
    ~Test_GainModule();

private slots:
    // Проверка работы выполнения
    void Operate();

private:

    void AddData(const csignal<double>& sig)
    {
        portIn.PushToOutput(sig);
    }

    // Функция получения данных модуля
    csignal<double> GetData()
    {
        module.Operate();
        csignal<double> sig;
        portOut.PopFromInput(sig);
        return sig;
    }

    // Входной порт тестируемого модуля
    OutputPort<csignal<double>> portIn;
    // Выходной порт тестируемого модуля
    InputPort<csignal<double>> portOut;
    // Тестируемый модуль
    GainModule module;
};

Test_GainModule::Test_GainModule(): portIn(nullptr), portOut(nullptr)
{
    portIn.Connect(module.GetInput(0));
    module.GetOutput(0)->Connect(&portOut);
}

Test_GainModule::~Test_GainModule(){}

void Test_GainModule::Operate()
{
    csignal<double> sig(10, 1);
    //Усиление по умолчанию
    AddData(sig);
    QCOMPARE(sig, GetData());
    //Проверка произвольного усиления
    module.SetGainSignal(10);
    AddData(sig);
    csignal<double> sig_10(10, 10);
    auto sig_10_res = GetData();
    QCOMPARE(sig_10, sig_10_res);
    // проверка на не соответствие с другим сигналом
    QVERIFY(!(sig_10_res == sig));
    // проверка на установку усиления 0
    module.SetGainSignal(0);
    AddData(sig);
    auto sig_0_res = GetData();
    csignal<double> sig_0(10, 0);
    QCOMPARE(sig_0, sig_0_res);
}

QTEST_APPLESS_MAIN(Test_GainModule)
#include "test_gainmodule.moc"
