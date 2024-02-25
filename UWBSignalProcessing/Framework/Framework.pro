TEMPLATE = lib
TARGET = UWBframework
CONFIG += c++17

QT += core gui widgets

API_PATH = $$PWD/..

INCLUDEPATH += \
    $${API_PATH}/DSPMLib \
    $${API_PATH}/DSPMLib/Tools \
    $${API_PATH}/MainWindow \
    $${API_PATH}/DialogBox \
    $${API_PATH}/Intefaces \
    $${API_PATH}/Tools

DEPENDPATH += \
    $${API_PATH}/DSPMLib \
    $${API_PATH}/DSPMLib/Tools \
    $${API_PATH}/MainWindow \
    $${API_PATH}/DialogBox \
    $${API_PATH}/Intefaces \
    $${API_PATH}/Tools

SOURCES += \
    $${API_PATH}/DSPMLib/abstractmodule.cpp \
    $${API_PATH}/DSPMLib/csignal.cpp \
    $${API_PATH}/DialogBox/customdialwidget.cpp \
    $${API_PATH}/MainWindow/abstractblock.cpp \
    $${API_PATH}/MainWindow/ioblock.cpp \
    $${API_PATH}/MainWindow/sourceblock.cpp \
    $${API_PATH}/MainWindow/sinkblock.cpp \
    $${API_PATH}/MainWindow/abstractdialogcommand.cpp \
    $${API_PATH}/Tools/timerevent.cpp \
    $${API_PATH}/MainWindow/lineconnect.cpp

HEADERS += \
    $${API_PATH}/DSPMLib/abstractmodule.h \
    $${API_PATH}/DSPMLib/iomodule.h \
    $${API_PATH}/DSPMLib/sinkmodule.h \
    $${API_PATH}/DSPMLib/sourcemodule.h \
    $${API_PATH}/DSPMLib/csignal.h \
    $${API_PATH}/Intefaces/bilderblocks.h \
    $${API_PATH}/Intefaces/abstractoperationmanager.h \
    $${API_PATH}/Intefaces/changeevent.h \
    $${API_PATH}/Intefaces/dialogwindow.h \
    $${API_PATH}/Intefaces/moduleconnectports.h \
    $${API_PATH}/DialogBox/customdialwidget.h \
    $${API_PATH}/MainWindow/abstractblock.h \
    $${API_PATH}/MainWindow/ioblock.h \
    $${API_PATH}/MainWindow/sinkblock.h \
    $${API_PATH}/MainWindow/sourceblock.h \
    $${API_PATH}/MainWindow/abstractdialogcommand.h \
    $${API_PATH}/Tools/timerevent.h \
    $${API_PATH}/MainWindow/lineconnect.h \
    $${API_PATH}/DSPMLib/portsconnect.h \
    $${API_PATH}/DSPMLib/datavrli.h \
    $${API_PATH}/Intefaces/savestruct.h \
    $${API_PATH}/DSPMLib/typesignal.h

DESTDIR = $${files(/home/*)}/UWB/
HEADERS_DESTDIR = $${DESTDIR}/include

headers.files = $$HEADERS
headers.path = $$HEADERS_DESTDIR
INSTALLS += headers

DEFINES += QT_DEPRECATED_WARNINGS
