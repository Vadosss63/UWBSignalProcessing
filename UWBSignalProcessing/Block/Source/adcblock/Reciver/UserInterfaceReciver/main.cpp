#include "rs485dialogbox.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[]) {

#ifndef HAVE_QT5
  QTextCodec::setCodecForCStrings(
      QTextCodec::codecForName("UTF-8")); //изменения
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8")); //изменения
#endif
  QApplication a(argc, argv);

  ReciverUWBDialog d(nullptr);
  d.show();

  return a.exec();
}
