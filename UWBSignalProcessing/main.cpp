#include "csignal.h"
#include "operationmanager.h"
#include "schemewindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTextCodec>

int main(int argc, char *argv[]) {

  QStringList paths = QCoreApplication::libraryPaths();
  paths.append(".");
  paths.append("imageformats");
  paths.append("platforms");
  QCoreApplication::setLibraryPaths(paths);

  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
  QApplication app(argc, argv);
  // QApplication::setQuitOnLastWindowClosed(false);

  SchemeWindow Window;
  csignal<double>::setAzimuthVolume(64);
  Window.SetOperationManager(new OperationManager);
  Window.AddViewOfSchema("schema1");
  Window.showNormal();
  //    Window.DEBAG();
  return QApplication::exec();
}
