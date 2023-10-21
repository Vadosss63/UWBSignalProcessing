#include "windows.h"

void Window::GetWindow(std::vector<double> &window, const size_t size,
                       TypeWindow varWindow) {
  window.resize(size);

  switch (varWindow) {
  case noWindow:
    for (size_t i = 0; i < size; ++i)
      window[i] = 1;
    break;
  case Hamming:
    for (size_t i = 0; i < size; ++i) {
      window[i] = (0.54 - 0.46 * std::cos(2 * M_PI * (double(i) / size)));
    }
    break;
  case Hann:
    for (size_t i = 0; i < size; ++i) {
      window[i] = 0.5 * (1 - std::cos(2 * M_PI * (double(i) / size)));
    }
    break;
  case Gaussian: {
    double alpha = 2.5; // параметр который можно тоже крутить
    for (size_t i = 0; i < size; ++i) {
      window[i] = std::exp(-0.5 * std::pow((alpha * ((i - (double(size) / 2)) /
                                                     (double(size) / 2))),
                                           2));
    }
  } break;
  case BartlettHanning:
    for (size_t i = 0; i < size; ++i) {
      window[i] = 0.62 - 0.48 * std::abs((double(i) / size) - 0.5) +
                  0.38 * std::cos(2 * M_PI * ((double(i) / size) - 0.5));
    }
    break;
  case Blackman:
    for (size_t i = 0; i < size; ++i) {
      window[i] = (0.42 - 0.5 * std::cos(2 * M_PI * (double(i) / size)) +
                   0.08 * std::cos(4 * M_PI * (double(i) / size)));
    }
    break;
  case Bartlett: {
    double size2 = (size / 2);
    for (size_t i = 0; i < size2; ++i) {
      window[i] = 2 * (double(i) / size);
    }

    for (size_t i = size2; i < size; ++i) {
      window[i] = 2 - (2 * (double(i) / size));
    }
  } break;
  }
}
