#ifndef TYPEWINDOW_H
#define TYPEWINDOW_H

#include <iostream>
#include <math.h>
#include <vector>

// windows
#define M_PI 3.14159265358979323846

enum TypeWindow {
  noWindow = 0,
  Hamming,
  Hann,
  Gaussian,
  BartlettHanning,
  Blackman,
  Bartlett
};

class Window {
public:
  static void GetWindow(std::vector<double> &window, const size_t size,
                        TypeWindow varWindow);
};

#endif // TYPEWINDOW_H
