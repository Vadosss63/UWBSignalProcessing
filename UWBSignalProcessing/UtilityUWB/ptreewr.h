#ifndef PTREEWR_H
#define PTREEWR_H

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <list>
#include <map>

namespace UtilityUWB {

// выполняет преобразования
class PtreeWR {
  typedef boost::property_tree::ptree Ptree;

public:
  PtreeWR();
  ~PtreeWR();

  // чтение дерева из стринга
  void ReadJson(std::string &&dataChar);
  void ReadJson(std::string &&dataChar, unsigned long activeBate);
  // выполняет запись в string-овый буфер
  std::string WriteToBuffer() const;

  // Очистить дерево
  void Clear();

  // чтение имени команды
  void AddName(const std::string &name);
  std::string ReadName() const;

  // запись и чтение параметров команды
  void AddParameter(const std::string &name, uint8_t val);
  void AddParameter(const std::string &, uint16_t val);
  void AddParameter(const std::string &, uint32_t val);
  void AddParameter(const std::string &, int8_t val);
  void AddParameter(const std::string &, int16_t val);
  void AddParameter(const std::string &, int32_t val);
  void AddParameter(const std::string &, double val);
  void AddParameter(const std::string &, std::string val);

  void AddError(const std::string &error);
  std::string ReadError();

  std::list<std::string> ParametersKey() const;
  template <typename Type>
  Type ReadParameterValue(const std::string &nameParameter);
  std::string ReadParameterType(const std::string &nameParameter);

private:
  template <typename Type>
  void AddParameterField(const std::string &name, const Type &val,
                         const std::string &type);

  // Обработчик JSON
  Ptree m_treeJson;
  // Используемые поля
  static constexpr const char *m_nameField = "Name";
  static constexpr const char *m_errorField = "Error";
  static constexpr const char *m_parametersField = "Parameters";

  static constexpr const char *m_valueField = "value";

  static constexpr const char *m_typeField = "type";
  static constexpr const char *m_typeDataUint8Field = "uint8_t";
  static constexpr const char *m_typeDataUint16Field = "uint16_t";
  static constexpr const char *m_typeDataUint32Field = "uint32_t";
  static constexpr const char *m_typeDataInt8Field = "int8_t";
  static constexpr const char *m_typeDataInt16Field = "int16_t";
  static constexpr const char *m_typeDataInt32Field = "int32_t";
  static constexpr const char *m_typeStringField = "string";
  static constexpr const char *m_typeDataDoubleField = "double";
};

template <typename Type>
void PtreeWR::AddParameterField(const std::string &name, const Type &val,
                                const std::string &type) {
  std::string pathValue = std::string(m_parametersField) + "." + name + "." +
                          std::string(m_valueField);
  m_treeJson.put(pathValue, val);
  std::string pathType = std::string(m_parametersField) + "." + name + "." +
                         std::string(m_typeField);
  m_treeJson.put(pathType, type);
}

template <typename Type>
Type PtreeWR::ReadParameterValue(const std::string &nameParameter) {
  std::string pathToParameter =
      std::string(m_parametersField) + "." + nameParameter + "." + m_valueField;
  Type value = m_treeJson.get<Type>(pathToParameter, Type());
  return value;
}

} // namespace UtilityUWB
#endif // PTREEWR_H
