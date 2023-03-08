#ifndef TYPEWINDOW_H
#define TYPEWINDOW_H

#include <iostream>
#include <vector>
#include <math.h>

//windows
#define M_PI 3.14159265358979323846

enum TypeWindow
{
    noWindow = 0,
    Hamming,
    Hann,
    Gaussian,
    BartlettHanning,
    Blackman,
    Bartlett
};

class Window
{
public:
    static void GetWindow(std::vector<double> &window, const size_t size, TypeWindow varWindow);

};

#endif // TYPEWINDOW_H
