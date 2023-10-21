#ifndef VRLISACANMODULE_H
#define VRLISACANMODULE_H

#include "csignal.h"
#include "datavrli.h"
#include "iomodule.h"
#include <algorithm>
#include <list>
#include <vector>

class VRLISacanModule
    : public IOModule<std::vector<csignal<double>>, std::vector<DataVRLI>> {
  struct Target {
    size_t angel;
    size_t range;
    float amp;
    //операторы сравнения
    bool operator<(const Target &target) { return amp < target.amp; }
    bool operator<(const Target &target) const { return amp < target.amp; }
    bool operator>(const Target &target) { return amp > target.amp; }
    bool operator>(const Target &target) const { return amp > target.amp; }
  };

public:
  VRLISacanModule();
  VRLISacanModule(const VRLISacanModule &rhs) = delete;
  ~VRLISacanModule() override = default;
  VRLISacanModule &operator=(const VRLISacanModule &rhs) = delete;

  void Operate() override;
  void StartEvent() override;

  void SetCreateVRLI(bool);
  void SetMaxIdexFind(size_t maxIdexFind);
  void SetThreshold(int threshold);
  void SetSigmaAngel(size_t sigmaAngel);
  void SetSigmaRange(size_t sigmaRange);

private:
  // набор скана из дорожек дальности
  void CheckScan();
  // поиск целей на каждом угле
  void FindMaxValForAngel();
  // поиск предпологаемых целей
  void FindTargetsCurrentIteration();
  // обнуление целей по сигмам в окресности
  void ClearTarget(size_t angel, size_t range, int val = 0);
  // Добавляет Цель на скан первички
  void AddTargetToScan(size_t angel, size_t range, int amp = 2048);

  // проверяем цель на пресечение с другими
  bool CheckColusion(const Target &target);
  //ищет цели пока не закончатся
  void FindTarget();
  // возвращает значение элемента скана
  inline double &GetValScan(const size_t &angelIndex, const size_t &rangeIndex);
  // отправка данных дальше
  void SendData();
  // Сброс скана в ноль
  void ResetScan();
  // Формирует ВРЛИ в сигнал
  void CreateDataVRLI();

  // количество углов в скане
  size_t m_countAngel = 64;
  // разброс цели по углу
  size_t m_sigmaAngel = 4;
  // разброс цели по дальности
  size_t m_sigmaRange = 4;
  // максимальный индекс поиска
  size_t m_maxIndexFind = 440; // 440 * 0.3 = 132 м
  // порог поиска
  int m_threshold = 10;
  // скан для обработки
  std::vector<csignal<double>> m_scan;
  // ищим все максимальные значния по углу
  std::list<Target> m_maxValForAngel;
  // распознанные цели
  std::list<Target> m_targets;
};

#endif // VRLISACANMODULE_H
