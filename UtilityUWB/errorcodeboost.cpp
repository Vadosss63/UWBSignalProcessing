#include "errorcodeboost.h"

namespace UtilityUWB
{

bool ErrorCodeBoost::IsNoError()const noexcept
{
    return !m_errorCode.value();
}

void ErrorCodeBoost::ClearErrors() noexcept
{
    m_errorCode.clear();
}

ErrorCode& ErrorCodeBoost::GetErrors() noexcept
{
    return  m_errorCode;
}

int ErrorCodeBoost::GetErrorsValue() const noexcept
{
    return  m_errorCode.value();
}

std::string ErrorCodeBoost::GetErrorsMessage() const noexcept
{
    return  m_errorCode.message();
}

}
