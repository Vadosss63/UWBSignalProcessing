#include "ptreewr.h"
#include <iostream>
#include <sstream>

namespace UtilityUWB {

PtreeWR::PtreeWR() {}

PtreeWR::~PtreeWR() { m_treeJson.clear(); }

void PtreeWR::ReadJson(std::string &&data) {
  m_treeJson.clear();
  std::istringstream streamBuf(std::move(data));
  boost::property_tree::read_json(streamBuf, m_treeJson);
}

void PtreeWR::ReadJson(std::string &&dataChar, unsigned long activeBate) {
  m_treeJson.clear();
  std::string data(dataChar.begin(), dataChar.begin() + activeBate);
  std::istringstream streamBuf(data);
  boost::property_tree::read_json(streamBuf, m_treeJson);
}

std::string PtreeWR::WriteToBuffer() const {
  std::ostringstream streamBuf;
  boost::property_tree::write_json(streamBuf, m_treeJson);
  return streamBuf.str();
}

void PtreeWR::AddName(const std::string &name) {
  m_treeJson.put(std::string(m_nameField), name);
}

std::string PtreeWR::ReadName() const {
  std::string name = m_treeJson.get<std::string>(m_nameField, std::string());
  return name;
}

std::list<std::string> PtreeWR::ParametersKey() const {
  std::list<std::string> parameters;
  auto param = m_treeJson.get_child_optional(m_parametersField);
  if (param) {
    for (auto &path : *param) {
      std::string name = path.first;
      parameters.emplace_back(name);
    }
  }
  return parameters;
}

std::string PtreeWR::ReadParameterType(const std::string &nameParameter) {
  std::string pathToParameter =
      std::string(m_parametersField) + "." + nameParameter + "." + m_typeField;
  std::string value =
      m_treeJson.get<std::string>(pathToParameter, std::string());
  return value;
}

void PtreeWR::Clear() { m_treeJson.clear(); }

void PtreeWR::AddParameter(const std::string &name, uint16_t val) {
  AddParameterField(name, val, m_typeDataUint16Field);
}

void PtreeWR::AddParameter(const std::string &name, uint32_t val) {
  AddParameterField(name, val, m_typeDataUint32Field);
}

void PtreeWR::AddParameter(const std::string &name, int8_t val) {
  AddParameterField(name, val, m_typeDataInt8Field);
}

void PtreeWR::AddParameter(const std::string &name, int16_t val) {
  AddParameterField(name, val, m_typeDataInt16Field);
}

void PtreeWR::AddParameter(const std::string &name, int32_t val) {
  AddParameterField(name, val, m_typeDataInt32Field);
}

void PtreeWR::AddParameter(const std::string &name, double val) {
  AddParameterField(name, val, m_typeDataDoubleField);
}

void PtreeWR::AddParameter(const std::string &name, std::string val) {
  AddParameterField(name, val, m_typeStringField);
}

void PtreeWR::AddError(const std::string &error) {
  m_treeJson.put(std::string(m_errorField), error);
}

std::string PtreeWR::ReadError() {
  auto name = m_treeJson.get<std::string>(m_errorField, std::string());
  return name;
}

void PtreeWR::AddParameter(const std::string &name, uint8_t val) {
  AddParameterField(name, val, m_typeDataUint8Field);
}

} // namespace UtilityUWB
