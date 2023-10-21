#ifndef ERRORCODEBOOST_H
#define ERRORCODEBOOST_H

#include <boost/system/error_code.hpp>
namespace UtilityUWB {

using ErrorCode = boost::system::error_code;

class ErrorCodeBoost {

public:
  // проверка на отсутствие ошибок
  bool IsNoError() const noexcept;
  void ClearErrors() noexcept;
  // возвращает ошибки
  ErrorCode &GetErrors() noexcept;
  int GetErrorsValue() const noexcept;
  std::string GetErrorsMessage() const noexcept;

private:
  ErrorCode m_errorCode;
};
} // namespace UtilityUWB
#endif // ERRORCODEBOOST_H
