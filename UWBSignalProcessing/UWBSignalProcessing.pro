TARGET = UWBSignalProcessing
TEMPLATE = app

CONFIG += rtti c++17
CONFIG += precompile_header

QT += core gui
QT += network xml sql
QT += widgets
DEFINES += HAVE_QT5

win32:RC_ICONS = icon.ico

unix:DESTDIR = $${files(/home/*)}/UWB
win32:DESTDIR = $$PWD/../UWB

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = stable.h

include(unix_libs.pri)
include(win_libs.pri)

INCLUDEPATH += $$PWD/../DialogBox
DEPENDPATH += $$PWD/../DialogBox

INCLUDEPATH += $$PWD/DSPMLib
DEPENDPATH += $$PWD/DSPMLib

INCLUDEPATH += $$PWD/DSPMLib/Tools
DEPENDPATH += $$PWD/DSPMLib/Tools

INCLUDEPATH += $$PWD/Tools
DEPENDPATH += $$PWD/Tools

INCLUDEPATH += $$PWD/MainWindow
DEPENDPATH += $$PWD/MainWindow

INCLUDEPATH += $$PWD/Intefaces
DEPENDPATH += $$PWD/Intefaces

HEADERS += \   
    Tools/operationmanager.h \
    stable.h \
    MainWindow/saveschematic.h \
    MainWindow/abstractblock.h \
    MainWindow/abstractdialogcommand.h \
    MainWindow/schemticsscene.h \
    MainWindow/textitem.h \
    MainWindow/databaseblocks.h \
    DSPMLib/abstractmodule.h \
    Intefaces/bilderblocks.h \
    Intefaces/abstractoperationmanager.h \
    Tools/timer.h \
    Tools/timerevent.h \
    MainWindow/lineconnect.h \
    Intefaces/changeevent.h \
    Intefaces/savestruct.h \
    Intefaces/moduleconnectports.h \
    MainWindow/selectpanel.h \
    MainWindow/schemewindow.h \
    MainWindow/schemaview.h \
    DSPMLib/Tools/target.h

SOURCES += \
    Tools/operationmanager.cpp \
    MainWindow/saveschematic.cpp \
    MainWindow/abstractblock.cpp \
    MainWindow/abstractdialogcommand.cpp \
    MainWindow/schemticsscene.cpp \
    MainWindow/textitem.cpp \
    main.cpp \     
    MainWindow/databaseblocks.cpp \
    DSPMLib/abstractmodule.cpp \
    Tools/timerevent.cpp \
    MainWindow/lineconnect.cpp \
    MainWindow/schemewindow.cpp \
    MainWindow/selectpanel.cpp \
    MainWindow/schemaview.cpp \
    DSPMLib/Tools/target.cpp

RESOURCES = UWBSignalProcessing.qrc
