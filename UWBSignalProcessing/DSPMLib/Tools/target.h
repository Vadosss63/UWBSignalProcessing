#ifndef TARGET_H
#define TARGET_H

#include <cstring>
#include <fstream>
#include <list>
#include <math.h>
#include <vector>

// windows
#define M_PI 3.14159265358979323846
// структура для сохранения массива возможных целей
class Target {
  // Класс для тестирование модуля
  friend class Test_Target;

public:
  struct Point {
    Point() = default;
    Point(int vSpeed, int vRange, double vAmp)
        : speed(vSpeed), distence(vRange), amp(vAmp) {}
    // скорость
    int speed = 0;
    // дистанция в отсчетах
    int distence = 0;
    // пелинг в градусах
    double direction = 0;
    // амплитуда
    double amp = 0;

    bool operator==(const Point &target) {
      return target.speed == speed && target.distence == distence;
    }

    bool operator==(const Point &target) const {
      return target.speed == speed && target.distence == distence;
    }
  };

  Target() = default;
  ~Target() = default;
  double GetDirection(size_t numberPoint);
  double GetX(const Point &point) const;
  double GetY(const Point &point) const;

  double GetSpeedKMH(size_t numberPoint = 0);
  double GetToSpeedKMH(size_t s) const;
  double GetAMP(size_t numberPoint = 0);
  void AddPoint(const Point &point);
  void RemovePoint(size_t numberPoint);
  Target::Point &GetPoint(size_t numberPoint);
  size_t Size() const;
  size_t Size();

  std::list<Point> &GetPoints();
  const std::list<Point> &GetPoints() const;
  Target::Point &GetFont();
  size_t GetCountAcumm() const;
  void SetCountAcumm(const size_t &countAcumm);
  static size_t GetMaxSpeed();
  static void SetMaxSpeed(const size_t &maxSpeed);

  size_t m_currentSpeed = 0;
  constexpr static const double RESULUTION = 0.3;

private:
  std::list<Target::Point>::iterator GetItter(size_t numberPoint);

  double GetSpeedMS(size_t s) const;
  std::list<Target::Point> m_pointTarget;
  // разрешение на один дискрет дальности
  static size_t m_maxSpeed;
  size_t m_countAcumm = 256;
};

//для чтения из файла
template <typename T> Target &operator>>(T &in, Target &target) {
  uint8_t countPoint = 0;
  in.read(reinterpret_cast<char *>(&countPoint), sizeof(countPoint));
  uint16_t currentSpeed = 0;
  in.read(reinterpret_cast<char *>(&currentSpeed), sizeof(currentSpeed));
  target.m_currentSpeed = currentSpeed;
  uint16_t countAccum = 0;
  in.read(reinterpret_cast<char *>(&countAccum), sizeof(countAccum));
  target.SetCountAcumm(countAccum);
  for (int i = 0; i < countPoint; ++i) {
    Target::Point point;
    in.read(reinterpret_cast<char *>(&(point.speed)), sizeof(point.speed));
    in.read(reinterpret_cast<char *>(&(point.distence)),
            sizeof(point.distence));
    in.read(reinterpret_cast<char *>(&(point.direction)),
            sizeof(point.direction));
    in.read(reinterpret_cast<char *>(&(point.amp)), sizeof(point.amp));

    target.AddPoint(point);
  }
  return target;
}

template <typename T>
//для записи в файл
void operator<<(T &out, const Target &target) {
  const std::list<Target::Point> &listPoints = target.GetPoints();
  uint8_t countPoint = static_cast<uint8_t>(listPoints.size());
  out.write(reinterpret_cast<char *>(&countPoint), sizeof(countPoint));
  uint16_t currentSpeed = static_cast<uint16_t>(target.m_currentSpeed);
  out.write(reinterpret_cast<char *>(&currentSpeed), sizeof(currentSpeed));
  uint16_t countAccum = static_cast<uint16_t>(target.GetCountAcumm());
  out.write(reinterpret_cast<char *>(&countAccum), sizeof(countAccum));

  for (const Target::Point &point : listPoints) {
    out.write(reinterpret_cast<char const *>(&(point.speed)),
              sizeof(point.speed));
    out.write(reinterpret_cast<char const *>(&(point.distence)),
              sizeof(point.distence));
    out.write(reinterpret_cast<char const *>(&(point.direction)),
              sizeof(point.direction));
    out.write(reinterpret_cast<char const *>(&(point.amp)), sizeof(point.amp));
  }
}

class GetTarget {
public:
  virtual ~GetTarget() = default;
  virtual void ReadTargets(std::vector<Target> &&) {}
};
#endif // TARGET_H
