#include <QtTest>

#include "../avtoelectronicablock/avtoelectronicamodule.h"

class Test_AvtoelectronicaModule : public QObject
{
    Q_OBJECT

public:
    Test_AvtoelectronicaModule();
    ~Test_AvtoelectronicaModule();

private slots:
    void Operate();
    void GetSpeed();
    void GetPhaseError();
};

Test_AvtoelectronicaModule::Test_AvtoelectronicaModule(){}

Test_AvtoelectronicaModule::~Test_AvtoelectronicaModule(){}

void Test_AvtoelectronicaModule::Operate()
{
    // Входной порт тестируемого модуля
    OutputPort<csignal<double>> portIn(nullptr);
    // Выходной порт тестируемого модуля
    InputPort<csignal<double>> portOut(nullptr);
    AvtoelectronicaModule module;
    module.Operate();
}

void Test_AvtoelectronicaModule::GetSpeed()
{
    FieldZone fz;
    double speed = 0.;
    double maxSpeed = 60;
    size_t countAccum = 256;
    fz.m_maxSpeed = maxSpeed;
    fz.m_countAcumm = countAccum;

    fz.m_currentSpeed = 0;
    QCOMPARE(fz.GetMySpeed(), speed);

    fz.m_currentSpeed = 240;
    speed = maxSpeed / (countAccum / 2) * 16;
    QCOMPARE(fz.GetMySpeed(), speed);

    fz.m_currentSpeed = 127;
    speed = maxSpeed / (countAccum / 2) * 127;
    QCOMPARE(fz.GetMySpeed(), -speed);

    fz.m_currentSpeed = 128;
    speed = maxSpeed / (countAccum / 2) * 128;
    QCOMPARE(fz.GetMySpeed(), speed);

    fz.m_currentSpeed = 255;
    speed = maxSpeed / (countAccum / 2) * 1;
    QCOMPARE(fz.GetMySpeed(), speed);

    fz.m_currentSpeed = 50;
    speed = maxSpeed / (countAccum / 2) * 50;
    QCOMPARE(fz.GetMySpeed(), -speed);

    fz.m_currentSpeed = 257;
    speed = 0;
    QCOMPARE(fz.GetMySpeed(), speed);
}

void Test_AvtoelectronicaModule::GetPhaseError()
{
    /// TODO
//    FieldZone fz;
//    double phi = 0.;
//    double error = 0.;
//    double az = 0.;
//    QCOMPARE(fz.GetAzAfterPhaseError(phi, error), az);

//    phi = -M_PI;
//    error = 0.;
//    az = round(phi * fz.GetAzimutConst() * 180 / M_PI);
//    QCOMPARE(fz.GetAzAfterPhaseError(phi, error), az);

//    phi = -M_PI/2;
//    error = 0.2618;
//    az = round((phi - error) * fz.GetAzimutConst() * 180 / M_PI);
//    QCOMPARE(fz.GetAzAfterPhaseError(phi, error), az);

    /// а если угол меньше чем -2 * M_PI?


//    phi = -2 * M_PI;
//    error = 0.2618;
//    az = ((phi - error + 2 * M_PI) * fz.GetAzimutConst() * 180 / M_PI);
//    QCOMPARE(fz.GetAzAfterPhaseError(phi, error), az);

}

QTEST_APPLESS_MAIN(Test_AvtoelectronicaModule)
#include "test_avtoelectronicamodule.moc"
