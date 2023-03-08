#include "vrliscanmodule.h"

VRLISacanModule::VRLISacanModule() : IOModule(1, 1)
{
    ResetScan();
}

void VRLISacanModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    if(!PopFromInput(m_scan))
        return;

    CheckScan();
}

void VRLISacanModule::CreateDataVRLI()
{
    size_t sizePRLISig = m_targets.size();
    std::vector<DataVRLI> prli;
    prli.reserve(sizePRLISig);
    uint32_t Nc = 0;
    for (auto& target : m_targets)
    {
        DataVRLI dataVRLI{};
        // номер цели
        dataVRLI.Nc = Nc++;
        ///TODO выполнить номировку к углу и дальности
        dataVRLI.angel= target.angel;
        dataVRLI.range = target.range;
        dataVRLI.amp = target.amp;
        prli.emplace_back(dataVRLI);
    }
    PushToOutput(prli);
}

void VRLISacanModule::SetSigmaRange(size_t sigmaRange)
{
    m_sigmaRange = sigmaRange;
}

void VRLISacanModule::SetSigmaAngel(size_t sigmaAngel)
{
    m_sigmaAngel = sigmaAngel;
}

void VRLISacanModule::SetThreshold(int threshold)
{
    m_threshold = threshold;
}

void VRLISacanModule::SetMaxIdexFind(size_t maxIdexFind)
{
    m_maxIndexFind = maxIdexFind;
}

void VRLISacanModule::SetCreateVRLI(bool ){}

void VRLISacanModule::SendData()
{
    CreateDataVRLI();
}

void VRLISacanModule::ResetScan()
{
    m_scan.clear();
    m_scan.resize(m_countAngel);
    m_targets.clear();
    m_maxValForAngel.clear();
}

void VRLISacanModule::CheckScan()
{
    FindTarget();
    SendData();
    ResetScan();
}

void VRLISacanModule::FindMaxValForAngel()
{
    // ищим максимальные значения по всем дорожкам
    for(size_t angel = 0; angel < m_scan.size(); ++angel)
    {
        // добавляем предпологаемую цель по углу
        // устанавливаем занчение на начало дорожки
        Target target{};
        target.angel = angel;
        target.range = 0;
        // выставляем погоговое значение для поиска цели
        /// TODO проверить
        // m_threshold = m_scan[angel].sum() / m_scan[angel].size();
        target.amp = m_threshold;

        // выполняем поиск цели до максимального значения
        for (size_t index = 1; index < m_maxIndexFind; ++index)
        {
            if(target.amp < GetValScan(angel, index))
            {
                // проверяем больше ли значение цели на дорожке дальности
                target.amp = GetValScan(angel, index);
                target.range = index;
            }
        }
        // Если значение цели больше порогового мы его
        // добавляем как предпологаямая цель
        if(target.amp > m_threshold)
            m_maxValForAngel.push_back(target);
    }
}

void VRLISacanModule::FindTargetsCurrentIteration()
{
    m_maxValForAngel.sort();
    // ищем все цели и обнуляем их
    while (!m_maxValForAngel.empty())
    {
        Target target = m_maxValForAngel.back();
        m_maxValForAngel.pop_back();

        /// TODO посмотреть как лучше
        /// удаляем цели
        ClearTarget(target.angel, target.range);
        // проверяем на пересечение с другими целями
        if(CheckColusion(target))
        {
            // добовляем в список
            m_targets.push_back(target);
        }
    }
}

void VRLISacanModule::ClearTarget(size_t angel, size_t range, int val)
{
    size_t beginRange = (range < m_sigmaRange) ? 0 : (range - m_sigmaRange);
    size_t endRange = (range + m_sigmaRange + 1) < m_scan[0].size() ? (range + m_sigmaRange + 1) : m_scan[0].size();

    size_t beginAngel = (angel < m_sigmaAngel) ? 0 : (angel - m_sigmaAngel);
    size_t endAngel = (angel + m_sigmaAngel + 1) < m_scan.size() ? (angel + m_sigmaAngel + 1) : m_scan.size();

    for (size_t angelIndex = beginAngel; angelIndex < endAngel; ++angelIndex)
    {
        for (size_t rangeIndex = beginRange; rangeIndex < endRange ; ++rangeIndex)
        {
            GetValScan(angelIndex, rangeIndex) = val;
        }
    }

}

void VRLISacanModule::AddTargetToScan(size_t angel, size_t range, int amp)
{
    size_t sigmaRange = 2;
    size_t sigmaAngel = 1;
    size_t beginRange = (range < sigmaRange) ? 0 : (range - sigmaRange);
    size_t endRange = (range + sigmaRange + 1) < m_scan[0].size() ? (range + sigmaRange + 1) : m_scan[0].size();

    size_t beginAngel = (angel < sigmaAngel) ? 0 : (angel - sigmaAngel);
    size_t endAngel = (angel + sigmaAngel + 1) < m_scan.size() ? (angel + sigmaAngel + 1) : m_scan.size();

    for (size_t angelIndex = beginAngel; angelIndex < endAngel; ++angelIndex)
    {
        for (size_t rangeIndex = beginRange; rangeIndex < endRange ; ++rangeIndex)
        {
            GetValScan(angelIndex, rangeIndex) = amp;
        }
    }
}

bool VRLISacanModule::CheckColusion(const Target& target)
{
    bool addTarget = true;
    for (auto& checkTarget : m_targets)
    {
        if(checkTarget.range == target.range)
        {
            addTarget = false;
            break;
        }
        int angelCheck = static_cast<int>(checkTarget.angel) - static_cast<int>(target.angel);
        int rangeCheck = static_cast<int>(checkTarget.range) - static_cast<int>(target.range);

        if((std::abs(angelCheck) < m_sigmaAngel) &&
                (std::abs(rangeCheck) < (m_sigmaRange)))
        {
            addTarget = false;
            break;
        }

    }
    return addTarget;
}

void VRLISacanModule::FindTarget()
{
    // ищем значение максимальные значения точек
    FindMaxValForAngel();
    // пока не будут найдены все цели ищим
    while (!m_maxValForAngel.empty())
    {
        // проверяем точки как цели
        FindTargetsCurrentIteration();
        // ищем значение максимальные значения точек
        FindMaxValForAngel();
    }
}

double& VRLISacanModule::GetValScan(const size_t& angelIndex, const size_t& rangeIndex)
{
    return m_scan[angelIndex][rangeIndex];
}

void VRLISacanModule::StartEvent()
{
    ResetScan();
}
