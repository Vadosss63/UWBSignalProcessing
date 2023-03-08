#include "avtoelectronicamodule.h"

AvtoelectronicaModule::AvtoelectronicaModule() : IOModule(1, 1){}

void AvtoelectronicaModule::SetSetting(size_t countAcumm, size_t startRange, size_t range, double threshol, double thresholDynamic)
{
    m_farField.SetCountAccum(countAcumm);
    m_farField.SetStartRange(startRange);
    m_farField.SetRange(range);
    m_farField.m_threshol = threshol;
    m_farField.m_thresholDynamic = thresholDynamic;
    m_farField.SetMZona(40);
    m_farField.SetPlan();
}

void FieldZone::SetCountAccum(size_t countAcumm)
{
    m_countAcumm = countAcumm;
}

void FieldZone::SetStartRange(size_t startRange)
{
    m_startRange = startRange;
}

void FieldZone::SetRange(size_t range)
{
    m_range = range;
}

void AvtoelectronicaModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    std::pair<csignal<double>, csignal<double>> sig;
    if(!PopFromInput(sig))
    {
        QueueOperation();
        return;
    }

    int currentAntenna = sig.first.frameNumber() % 2;

    switch (currentAntenna) {
    case 0:
        m_farField.AccumSigFirstAntenna(sig);
        m_farField.m_currentAccumSignalFirstAntenna++;
        break;
    case 1:
        m_farField.AccumSigSecondAntenna(sig);
        m_farField.m_currentAccumSignalSecondAntenna++;
        break;
    default:
        break;
    }

    // Выполняем накопление

    bool isExecute1 = m_farField.m_countAcumm <= m_farField.m_currentAccumSignalSecondAntenna;
    bool isExecute2 = m_farField.m_countAcumm <= m_farField.m_currentAccumSignalFirstAntenna;
    if(isExecute1 && isExecute2)
    {
        OutBuffer outBufferFar = m_farField.Execute();
        //OutBuffer outBufferFar = m_farField.ExecuteRAWData();
        m_farField.ResetSignal();
        PushToOutput(std::move(outBufferFar));
    }
}

void AvtoelectronicaModule::StartEvent()
{
    m_farField.ResetSignal();
    m_farField.m_isInitThreshol = false;
    std::cout << __PRETTY_FUNCTION__;
    std::cout << std::endl;
}

void AvtoelectronicaModule::StopEvent()
{
    std::cout << __PRETTY_FUNCTION__;
    std::cout << std::endl;
    IOModule::StopEvent();
}

void FieldZone::AccumSigFirstAntenna(std::pair<csignal<double>, csignal<double>>& sig)
{
    if(m_currentAccumSignalFirstAntenna < m_countAcumm)
    {
        // выполняем накопление
        for (size_t range = m_startRange; range < (m_startRange + m_range); ++range)
        {
            double real = sig.first[range];
            double im = sig.second[range];
            size_t index = range - m_startRange;
            m_accumSignalFirstAntenna[index][m_currentAccumSignalFirstAntenna] = std::complex<double>(real, im);
        }
    }
}

void FieldZone::AccumSigSecondAntenna(std::pair<csignal<double>, csignal<double> >& sig)
{
    if(m_currentAccumSignalSecondAntenna < m_countAcumm)
    {
        // выполняем накопление
        for (size_t range = m_startRange; range < (m_startRange + m_range); ++range)
        {
            double real = sig.first[range];
            double im = sig.second[range];
            size_t index = range - m_startRange;
            m_accumSignalSecondAntenna[index][m_currentAccumSignalSecondAntenna] = std::complex<double>(real, im);
        }
    }
}

void FieldZone::ResetSignal()
{
    m_currentAccumSignalFirstAntenna = 0;
    m_currentAccumSignalSecondAntenna = 0;
}

double FieldZone::MeanValue() const
{
    double mean = 0;
    int size = 0;
    for (size_t range = m_mZona; range < m_accumSignalFirstAntenna.size(); ++range) {
        for (size_t speed = 0; speed < m_accumSignalFirstAntenna[range].size(); ++speed) {
            if(speed != m_currentSpeed)
            {
                mean += GetAbs(range, speed);
                size++;
            }
        }
    }
    return mean / size;
}

double FieldZone::MeanValueWithTreashold() const
{
    double mean = 0;
    int size = 0;
    double abs = 0;
    for (size_t range = m_mZona; range < m_accumSignalFirstAntenna.size(); ++range) {
        for (size_t speed = 0; speed < m_accumSignalFirstAntenna[range].size(); ++speed) {
            if(speed != m_currentSpeed)
            {
                abs = GetAbs(range, speed);
                if(abs >  m_thresholdSpeed)
                    mean += m_thresholdSpeed;
                else
                    mean += abs;
                size++;
            }
        }
    }
    return mean / size;
}


double FieldZone::Variance(double mean)
{
    double variance = 0;
    int size = 0;
    size_t sizeRange = m_accumSignalFirstAntenna.size();
    size_t sizeSpeed = m_accumSignalFirstAntenna[0].size();
    for (size_t range = m_mZona; range < sizeRange; range++) {
        for (size_t speed = 0; speed < sizeSpeed; speed++) {
            if(speed != m_currentSpeed)
            {
                variance += std::pow(GetAbs(range, speed) - mean, 2);
                size++;
            }
        }
    }

    return variance / (size - 1);
}

size_t FieldZone::GetMyCurrentSpeed() const
{
    //std::vector<double> mean = MeanValueWithTreasholdFromSpeed();
    double mean = MeanValueWithTreashold();
    size_t speedSize = m_accumSignalFirstAntenna[0].size();
    std::vector<double> deviation(speedSize);

    double absval = 0;
    for (size_t speed = 0; speed < speedSize; ++speed) {
        for (size_t range = m_mZona; range < m_accumSignalFirstAntenna.size(); ++range) {
            absval = GetAbs(range, speed);
            if(absval  >=   m_thresholdSpeed)
                deviation[speed] += std::pow(m_thresholdSpeed - mean, 2);
            else
                deviation[speed] += std::pow(absval - mean, 2);
            //            deviation[speed] += std::pow(absval - mean, 2);
        }
        double size = (m_accumSignalFirstAntenna.size() - m_mZona) * m_accumSignalFirstAntenna[0].size();
        deviation[speed] = std::sqrt(deviation[speed] / (size - 1));
    }

    size_t currentSpeed = static_cast<size_t>(std::max_element(deviation.begin(), deviation.end()) - deviation.begin());
    return currentSpeed;
}

double FieldZone::GetMySpeed()
{
    if(m_currentSpeed >= m_countAcumm )
        return 0;

    if(m_currentSpeed >= (m_countAcumm / 2))
        return (2 * (m_countAcumm - m_currentSpeed) * m_maxSpeed) / m_countAcumm;


    return -(2 * m_currentSpeed * m_maxSpeed) / m_countAcumm;
}

void FieldZone::SetMZona(size_t mZona)
{
    m_mZona = mZona;
}

void FieldZone::SetMaxSpeed(double maxSpeed)
{
    m_maxSpeed = maxSpeed;
    Target::SetMaxSpeed(static_cast<int>(maxSpeed));
}

double FieldZone::GetAzimutConst()
{
    return LAMBDA / (2 * M_PI * BASE_ANTENNA);
}

FieldZone::FieldZone()
{
    BASE_ANTENNA = 0.328;
    CONST_TO_AZIMUT = GetAzimutConst();
    m_maxSpeed = 60;
    MAX_DETECT_AZIMUT = (LAMBDA/(2 * BASE_ANTENNA) - M_PI / 180) * 180 / M_PI;
    ResetSignal();
    SetPlan();
}

FieldZone::~FieldZone()
{
    ClearPlan();
}

void FieldZone::SetBaseAntenna(double value)
{
    BASE_ANTENNA = value;
    CONST_TO_AZIMUT = GetAzimutConst();
    MAX_DETECT_AZIMUT = (LAMBDA/(2 * BASE_ANTENNA) - M_PI / 180) * 180 / M_PI;;
}

std::vector<Target> FieldZone::Execute()
{
    for (size_t i = 0; i < m_range; ++i)
    {
        fftw_execute(plan_fftFirstAntenna[i]);
        fftw_execute(plan_fftSecondAntenna[i]);
    }

    // Обнуление мертвой зоны
    // Мертвая зона всегда будет иметь нулевую скорость,
    // так как импульс не перемещается относительно передающей антенны
    for (size_t range = 0; range < m_mZona; ++range)
        m_accumSignalFirstAntenna[range][0] = 0;

    if(!m_isInitThreshol)
    {
        m_threshol = GetThreshol();
        m_thresholdSpeed = m_threshol*10;
        m_thresholDynamic = m_threshol;
        std::cout << "Порог = " << m_threshol << std::endl;
        std::cout << "СКО шума = " << GetStandartDeviation()<< std::endl;
        m_isInitThreshol = true;
    }

    //  m_currentSpeed = GetMyCurrentSpeed();

    for (size_t speed = 0; speed < m_countAcumm; ++speed) {
        CheckTarget(speed);
        FindTarget();
    }

    std::vector<Target> vt;
    for(Target& tar : m_target)
    {
        tar.m_currentSpeed = m_currentSpeed;
        for (Target::Point& point : tar.GetPoints())
        {

            point.direction = GetAzimutTargets(static_cast<size_t>(point.distence),
                                               static_cast<size_t>(point.speed));


            point.amp += GetABSVal(point.distence, point.speed, 2);
            point.amp /= 2;
            point.amp = std::sqrt(point.amp);
            point.speed = point.speed - m_currentSpeed;

        }
        auto it = std::remove_if(tar.GetPoints().begin(),tar.GetPoints().end(),
                                 [this](Target::Point& p){
                double value = GetABSVal(static_cast<size_t>(p.distence), static_cast<size_t>(p.speed), 2);
                if(value < m_threshol)
                return true;

                return IsTargetErrorZone(p.direction);
    });

        tar.GetPoints().erase(it, tar.GetPoints().end());
        if(!tar.GetPoints().empty())
            vt.push_back(tar);
    }

    m_target.clear();
    m_listTargets.clear();
    return vt;
}

std::vector<std::vector<double>> FieldZone::ExecuteRAWData()
{
    std::vector<std::vector<double>> data = std::vector<std::vector<double>>(256, std::vector<double>(512));

    for (size_t i = 0; i < m_range; ++i)
    {
        fftw_execute(plan_fftFirstAntenna[i]);
        fftw_execute(plan_fftSecondAntenna[i]);
    }

    for (size_t range = 0; range < m_mZona; ++range)
        m_accumSignalFirstAntenna[range][0] = 0;

    for (size_t speed = 0; speed < 128; ++speed) {
        for (size_t range = 0; range < 512; ++range) {
            data[speed + 128][range] = GetABSVal(range, speed, 1) * 10E-8;
        }
    }

    for (size_t speed = 128; speed < 255; ++speed) {
        for (size_t range = 0; range < 512; ++range) {
            data[speed - 128][range] = GetABSVal(range, speed, 1) * 10E-8;
        }
    }

    return data;
}


bool FieldZone::IsTargetErrorZone(double az)
{
    return (MAX_DETECT_AZIMUT / 2 - std::abs(az)) < 0;
}

void FieldZone::FindTarget()
{
    if(m_listTargets.empty())
        return;


    std::list<Target::Point>& targetSpeed = m_listTargets;

    // забираем первую цель
    Target::Point firstTarget = targetSpeed.front();
    // удаляем значение из списка
    targetSpeed.remove(firstTarget);
    // добавляем в список
    m_target.emplace_back();
    m_target.back().AddPoint(firstTarget);

    int lastDistence = firstTarget.distence;

    while (!targetSpeed.empty()) {

        // забираем новую цель
        Target::Point currentTarget = targetSpeed.front();
        // удаляем значение из списка
        targetSpeed.remove(currentTarget);

        // проверка протяженности
        if(lastDistence + 1 == currentTarget.distence )
        {
            lastDistence = currentTarget.distence;
            Target::Point point;
            point.amp = currentTarget.amp;
            point.speed = currentTarget.speed;
            point.distence = currentTarget.distence;
            m_target.back().AddPoint(point);
        }
        else
        {
            m_target.emplace_back();
            m_target.back().AddPoint(currentTarget);
            lastDistence = currentTarget.distence;
        }
    }
}


double FieldZone::GetAzimutTargets(size_t range, size_t speed) const
{
    if(range >= m_accumSignalFirstAntenna.size())
        return 0;

    if(speed >= m_accumSignalFirstAntenna[0].size())
        return 0;

    const std::complex<double>& sig_1 = m_accumSignalFirstAntenna[range][speed];
    const std::complex<double>& sig_2 = m_accumSignalSecondAntenna[range][speed];

    double phiSig1 = std::arg(sig_1);
    double phiSig2 = std::arg(sig_2);

    double phiD = phiSig1 - phiSig2;
    double error = m_SpeedPhiErrors[speed];
    //double error = 0;
    return GetAzAfterPhaseError(phiD, error);

}

void FieldZone::CheckTarget(size_t speed)
{
    m_listTargets.clear();
    // проходим по всем дальностям
    for (size_t range = 0; range < m_range; ++range)
    {
        double amp = GetABSVal(range, speed);
        if(speed == m_currentSpeed) // для статических
        {
            if(amp > m_threshol)
                m_listTargets.emplace_back(static_cast<int>(speed), static_cast<int>(range), amp);
        }
        else // для динамических
        {
            if(amp > m_thresholDynamic)
                m_listTargets.emplace_back(static_cast<int>(speed), static_cast<int>(range), amp);
        }
    }
}

double FieldZone::GetABSVal(size_t range, size_t speed, size_t numberAntenna) const
{
    double val = GetAbs(range, speed, numberAntenna);

    if(m_isVARU)
    {
        if(range <= m_maxRange)
            val *= std::pow(static_cast<double> (range), 2) / m_varuD;
        else
            val *= 1;
    }


    if(m_currentSpeed == speed)
    {
        //if(range > m_maxRange) return  val;

        if(m_gain != 1.0)
            return  val * m_gain;
    }
    else
    {
        //if(range > m_maxRange) return  val;

        if(m_gainDynamic != 1.0)
            return  val * m_gainDynamic;
    }

    return val;
}

double FieldZone::GetAbs(size_t range, size_t speed, size_t numberAntenna) const
{
    if(numberAntenna == 1)
    {
        double val = std::pow(std::real(m_accumSignalFirstAntenna[range][speed]), 2)
                + std::pow(std::imag(m_accumSignalFirstAntenna[range][speed]), 2);
        if(speed == 0)
        {
            val -= 1.7 * std::pow(10, 9);
            if(val < 0)
                val = 0;
        }
        return val;
    }

    if(numberAntenna == 2)
    {
        double val = std::pow(std::real(m_accumSignalSecondAntenna[range][speed]), 2)
                + std::pow(std::imag(m_accumSignalSecondAntenna[range][speed]), 2);
        if(speed == 0)
        {
            val -= 1.7 * std::pow(10, 9);
            if(val < 0)
                val = 0;
        }
        return val;
    }
    return 0;
}

double FieldZone::GetAzAfterPhaseError(double phiD, double error) const
{
    phiD += error;
    if (phiD < -M_PI)
        phiD += 2 * M_PI;

    if (phiD > M_PI)
        phiD -= 2 * M_PI;

    // Перевод в градусы
    double azimuth = phiD * CONST_TO_AZIMUT * 180 / M_PI;
    return azimuth;
}

void FieldZone::CreateDeltaSpeedPhi()
{
    m_SpeedPhiErrors.clear();
    m_SpeedPhiErrors.resize(m_countAcumm);
    double F_povt = 4.0 * m_maxSpeed / LAMBDA;
    double T_povt = 1.0 / F_povt;
    double d_speed = (2 * m_maxSpeed) / m_countAcumm;
    int K_antenn = 2;

    for (size_t i = 0; i < m_countAcumm / 2; ++i)
    {
        double d_R = (d_speed * i) * T_povt / K_antenn;
        m_SpeedPhiErrors[i] = (2 * 2 * M_PI * d_R)/ LAMBDA;
    }

    for (size_t i = 1; i < m_countAcumm / 2; ++i) {
        m_SpeedPhiErrors[m_countAcumm - i] = -m_SpeedPhiErrors[i];
    }
    size_t i = m_countAcumm / 2 + 1;
    double d_R = (d_speed * i) * T_povt / K_antenn;
    m_SpeedPhiErrors[i] = -(2 * 2 * M_PI * d_R)/ LAMBDA;
}

double FieldZone::GetThreshol()
{
    double meanValue = MeanValue();
    double variance = Variance(meanValue);
    // по критерию Неймана Пирсона
    double dev = sqrt(variance);
    // распределения
    double c_f = myErfInv2(1 - m_pLT);
    double threshol = meanValue + c_f * dev;
    return threshol;
}

double FieldZone::GetStandartDeviation()
{
    double meanValue = MeanValue();
    double variance = Variance(meanValue);// дисперсия
    return sqrt(variance);
}

void AvtoelectronicaModule::SetGain(double gain)
{
    m_farField.m_gain = gain;
}

void AvtoelectronicaModule::SetGainDynamic(double gain)
{
    m_farField.m_gainDynamic = gain;

}

void AvtoelectronicaModule::SetBaseAntenna(double value)
{
    m_farField.SetBaseAntenna(value);
}

void AvtoelectronicaModule::SetMaxSpeed(double maxSpeed)
{
    m_farField.SetMaxSpeed(maxSpeed);
}

void AvtoelectronicaModule::SetRangeLook(int xBegin, int xEnd, int yBegin, int yEnd)
{
    m_xBegin = xBegin;
    m_xEnd = xEnd;
    m_yBegin = yBegin;
    m_yEnd = yEnd;
    m_checkLookZone = true;
}

void AvtoelectronicaModule::SetIsVARU(bool isVARU)
{
    m_farField.m_isVARU = isVARU;
}

void FieldZone::ClearPlan()
{
    for (auto& p : plan_fftFirstAntenna)
        fftw_destroy_plan(p);
    for (auto& p : plan_fftSecondAntenna)
        fftw_destroy_plan(p);
}

void FieldZone::SetPlan()
{
    // Расчет угловых ошибок для скоростных целей
    CreateDeltaSpeedPhi();

    if(m_accumSignalFirstAntenna.size() == m_range &&
            m_accumSignalFirstAntenna.front().size() == m_countAcumm)
        return;

    // Количество накоплений
    m_accumSignalFirstAntenna.resize(m_range);
    m_accumSignalSecondAntenna.resize(m_range);

    m_varuD = std::pow(static_cast<double>(m_maxRange), 2);

    if(!plan_fftFirstAntenna.empty())
        ClearPlan();

    plan_fftFirstAntenna.resize(m_range);
    plan_fftSecondAntenna.resize(m_range);

    // Диапазон перобразований
    for (size_t i = 0; i < m_range; ++i)
    {
        // создаем план
        m_accumSignalFirstAntenna[i].resize(m_countAcumm);
        plan_fftFirstAntenna[i] = fftw_plan_dft_1d(static_cast<int>(m_countAcumm), reinterpret_cast<fftw_complex*>(&m_accumSignalFirstAntenna[i][0]),
                reinterpret_cast<fftw_complex*>(&m_accumSignalFirstAntenna[i][0]), FFTW_FORWARD, FFTW_ESTIMATE);

        m_accumSignalSecondAntenna[i].resize(m_countAcumm);
        plan_fftSecondAntenna[i] = fftw_plan_dft_1d(static_cast<int>(m_countAcumm), reinterpret_cast<fftw_complex*>(&m_accumSignalSecondAntenna[i][0]),
                reinterpret_cast<fftw_complex*>(&m_accumSignalSecondAntenna[i][0]), FFTW_FORWARD, FFTW_ESTIMATE);

    }
}

float myErfInv2(float x){
    float tt1, tt2, lnx, sgn;
    sgn = (x < 0) ? -1.0f : 1.0f;

    x = (1 - x)*(1 + x);        // x = 1 - x*x;
    lnx = logf(x);

    tt1 = 2/(M_PI*0.147) + 0.5f * lnx;
    tt2 = 1/(0.147) * lnx;

    return(sgn*sqrtf(-tt1 + sqrtf(tt1*tt1 - tt2)));
}
