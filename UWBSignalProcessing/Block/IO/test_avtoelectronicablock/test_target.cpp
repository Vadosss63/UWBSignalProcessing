#include <QtTest>

#include "target.h"

class Test_Target : public QObject
{
    Q_OBJECT

public:
    Test_Target();
    ~Test_Target();

private slots:
    void GetSpeedMS();
    void GetSpeedKMH();

};

Test_Target::Test_Target(){}

Test_Target::~Test_Target(){}

void Test_Target::GetSpeedMS()
{
//    Target target;
//    double speed = 0.;
//    double maxSpeed = 60;
//    size_t countAccum = 256;
//    target.m_maxSpeed = maxSpeed;
//    target.m_countAcumm = countAccum;

//    target.m_currentSpeed = 0;
//    QCOMPARE(target.GetSpeedMS(target.m_currentSpeed), speed);

//    target.m_currentSpeed = 240;
//    speed = maxSpeed / (countAccum / 2) * 16;
//    QCOMPARE(target.GetSpeedMS(target.m_currentSpeed), speed);

//    target.m_currentSpeed = 127;
//    speed = maxSpeed / (countAccum / 2) * 127;
//    QCOMPARE(target.GetSpeedMS(target.m_currentSpeed), -speed);

//    target.m_currentSpeed = 128;
//    speed = maxSpeed / (countAccum / 2) * 128;
//    QCOMPARE(target.GetSpeedMS(target.m_currentSpeed), speed);

//    target.m_currentSpeed = 255;
//    speed = maxSpeed / (countAccum / 2) * 1;
//    QCOMPARE(target.GetSpeedMS(target.m_currentSpeed), speed);

//    target.m_currentSpeed = 50;
//    speed = maxSpeed / (countAccum / 2) * 50;
//    QCOMPARE(target.GetSpeedMS(target.m_currentSpeed), -speed);

//    target.m_currentSpeed = 257;
//    speed = 0;
//    QCOMPARE(target.GetSpeedMS(target.m_currentSpeed), speed);
}


void Test_Target::GetSpeedKMH(){
//    Target target;
//    target.speed = 0;
//    double speedKMH = 0;
//    QCOMPARE(target.GetSpeedKMH(), speedKMH);

//    target.speed = 5;
//    speedKMH = 3.6 * target.GetSpeedMS(target.speed);
//    QCOMPARE(target.GetSpeedKMH(), speedKMH);


//    target.speed = -2;
//    speedKMH = 0;
//    QCOMPARE(target.GetSpeedKMH(), speedKMH);

//    size_t speed = 0;
//    speedKMH = 0;
//    QCOMPARE(target.GetSpeedKMH(speed), speedKMH);

//    speed = 5;
//    speedKMH = 3.6 * target.GetSpeedMS(speed);
//    QCOMPARE(target.GetSpeedKMH(speed), speedKMH);


//    speed = -2;
//    speedKMH = 0;
//    QCOMPARE(target.GetSpeedKMH(speed), speedKMH);

}


//QTEST_APPLESS_MAIN(Test_Target)
#include "test_target.moc"
