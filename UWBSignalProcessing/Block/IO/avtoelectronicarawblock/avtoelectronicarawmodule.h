#ifndef AVTOELECTRONICARAWMODULE_H
#define AVTOELECTRONICARAWMODULE_H

#include <fftw3.h>
#include <list>
#include <map>
#include "iomodule.h"
#include "csignal.h"

using OutBuffer = std::vector<std::vector<double>>;

float myErfInv2(float x);

class FieldZone
{

    friend class AvtoelectronicaRawModule;
    friend class Test_AvtoelectronicaModule;

public:

    FieldZone();
    ~FieldZone();

    void SetMZona(size_t mZona);

private:

    std::vector<std::vector<double>> ExecuteRAWData();

    void SetBaseAntenna(double value);
    void SetMaxSpeed(double maxSpeed);
    double GetAzimutConst();

    void AccumSigFirstAntenna(std::pair<csignal<double>, csignal<double>>& sig);
    void AccumSigSecondAntenna(std::pair<csignal<double>, csignal<double>>& sig);

    void SetCountAccum(size_t countAcumm);
    void SetStartRange(size_t startRange);
    void SetRange(size_t range);
    void ResetSignal();
    void SetPlan();
    void ClearPlan();

    // Удаление целей из зоны неопределенности
    bool IsTargetErrorZone(double az);

    // Получение значенич азимута с определенной дальности
    inline double GetAzimutTargets(size_t range, size_t speed) const;
    inline double GetABSVal(size_t range, size_t speed, size_t numberAntenna = 1) const;
    double GetAbs(size_t range, size_t speed, size_t numberAntenna = 1) const;
    inline double GetAzAfterPhaseError(double phi, double error) const;
    // Расчет вектора ошибок
    void CreateDeltaSpeedPhi();

    // получение порогового значения для обработки
    double GetThreshol();
    // получение СКО шума
    double GetStandartDeviation();
    // Вычисление среднего значения
    double MeanValue() const;
    double MeanValueWithTreashold() const;
    // Дисперсия
    double Variance(double mean = 0);
    //Определение собственной скорости движения
    size_t GetMyCurrentSpeed() const;
    double GetMySpeed();

    size_t m_currentSpeed = 0;
    // порог для статических целей
    double m_threshol = 700;
    // порог для статических целей
    double m_thresholdSpeed = 700;
    // порог для динамических целей
    double m_thresholDynamic = 200;
    // начальное значение накопления
    size_t m_startRange = 0;
    // диапазон дальностей
    size_t m_range = 1;
    // максимальная дальность для вару
    size_t m_maxRange = 256;
    // количество накоплений
    size_t m_countAcumm = 256;
    // мертвая зона
    size_t m_mZona = 0;

    // буффер для накопления для первой антенны
    std::vector<std::valarray<std::complex<double>>> m_accumSignalFirstAntenna;
    // буффер для накопления для второй антенны
    std::vector<std::valarray<std::complex<double>>> m_accumSignalSecondAntenna;
    // набор планов преобразования fft
    std::vector<fftw_plan> plan_fftFirstAntenna;
    std::vector<fftw_plan> plan_fftSecondAntenna;

    // ошибки фазовых углов для скоростных целей
    std::vector<double> m_SpeedPhiErrors;

    double m_gain = 1.0;
    // откланение от максимальной амплитуды по дальности
    double m_deltaMaxAmp = 0.75;
    double m_gainDynamic = 1.0;
    // глубина вару
    double m_varuD = 1;
    // максимальная скорость измерения
    double m_maxSpeed; // м/с
    double BASE_ANTENNA;
    double CONST_TO_AZIMUT;
    double MAX_DETECT_AZIMUT;// Значениие азимута в зоне определенности

    // Для перевода набег фаз к углу
    const double LAMBDA = 0.104;

    // Вероятностные характеристики
    double m_pLT = 1E-3; // вероятность ложной тревоги
    const int K_ANTENN = 2;

    // Значение текущего накопления
    volatile uint16_t m_currentAccumSignalFirstAntenna = 0;
    volatile uint16_t m_currentAccumSignalSecondAntenna = 0;

    bool m_isInitThreshol = false;
    bool m_isVARU = false;
};

class AvtoelectronicaRawModule: public IOModule<std::pair<csignal<double>, csignal<double>>,  OutBuffer>
{
    // Класс для тестирование модуля
    friend class Test_AvtoelectronicaModule;
public:

    AvtoelectronicaRawModule();
    ~AvtoelectronicaRawModule() override = default;

    void Operate() override;
    void StartEvent() override;
    void StopEvent() override;

    void SetSetting(size_t countAcumm, size_t startRange, size_t range, double threshol, double thresholDynamic);
    void SetIsVARU(bool isVARU);
    void SetGain(double gain);
    void SetGainDynamic(double gain);

    void SetBaseAntenna(double value);
    void SetMaxSpeed(double maxSpeed);
    // Установка зраницы обзора
    void SetRangeLook(int xBegin, int xEnd, int yBegin, int yEnd);

private:
    FieldZone m_farField;
    int m_xBegin = 0;
    int m_xEnd = 0;
    int m_yBegin = 0;
    int m_yEnd = 0;
    bool m_checkLookZone = false;

};

#endif // AVTOELECTRONICARAWMODULE_H
