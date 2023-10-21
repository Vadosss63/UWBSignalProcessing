#ifndef INTERFACE_H
#define INTERFACE_H

#include <vector>

class UpdateData {

public:
  virtual ~UpdateData() = default;
  virtual void AddData(std::vector<double> &&data, int indexZ) = 0;
  virtual void ResetData() = 0;
  virtual void Update() = 0;
};

#endif // INTERFACE_H
