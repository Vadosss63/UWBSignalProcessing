#include <QApplication>
#include <algorithm>
#include <chrono>
#include <thread>
#include <oglpyramid.h>
#include "drawline.h"
#include "drawline3d.h"
#include "displayview.h"
#include "scope.h"


void TestThread(Scope& scope)
{
    int i = 0;
    size_t size = 8192;
    while (true) {

        std::vector<u_int16_t> data(size, i += 10);
        scope.AddData(data, 0);
        std::vector<u_int16_t> data2(size, i += 3);
        scope.AddData(data2, 1);
        scope.ReadyData();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

}

//    size_t size = 8192;
//    Scope drawLine(2, QSize(size, 1000));
//    drawLine.show();
//    std::thread t(TestThread, std::ref(drawLine));
//    t.detach();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DisplayView displayView;
    displayView.resize(500, 800);
    displayView.AddView(new DrawLine);
    displayView.AddViewOrig(new DrawLine);
    displayView.AddScope(new ScopeWidgetOPGL);
    displayView.show();
    return a.exec();
}
