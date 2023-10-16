#include "vrliviewmodule.h"

VRLIViewModule::VRLIViewModule(QObject* parent)
    :QObject (parent), SinkModule(1){}


void VRLIViewModule::Operate()
{
    std::lock_guard<std::mutex> lock(operateLock);

    if (!PopFromInput(m_dataTarget))
        return;

    PasingTarget();
}

std::vector<VRLIItem*>& VRLIViewModule::GetVRLIItem()
{
    std::lock_guard<std::mutex> lock(operateLock);

    return m_VRLIItem;
}

void VRLIViewModule::SetCoeffRange(float coeffRange)
{
    m_coeffRange = coeffRange;
}

void VRLIViewModule::PasingTarget()
{
    if(m_VRLIItem.empty())
    {
        m_VRLIItem.reserve(m_dataTarget.size());
        for(DataVRLI& target : m_dataTarget)
        {
            VRLIItem* item = new VRLIItem(m_startAngel + target.angel * m_coeffAngel, target.range * m_coeffRange, target.amp);
            m_VRLIItem.push_back(item);
        }

        ScanUpdated();
    }
}
