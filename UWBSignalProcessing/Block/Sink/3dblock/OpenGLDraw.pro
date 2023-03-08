TEMPLATE = app

QT += opengl

CONFIG += c++11

TARGET = OpenGLDraw

SOURCES += main.cpp \
    oglquad.cpp \
    ogldraw.cpp \
    oglpyramid.cpp \
    drawline.cpp \
    arrayvertex.cpp \
    arraydata3d.cpp \
    drawline3d.cpp \
    displayview.cpp \
    scopewidgetopgl.cpp \
    drawopglwidget.cpp \
    scope.cpp
#    widgetopengl.cpp

HEADERS += \
    oglquad.h \
    ogldraw.h \
    oglpyramid.h \
    drawline.h \
    arrayvertex.h \
    ../ParserData/dataparser.h \
    ../ParserData/header.h \
    arraydata3d.h \
    drawline3d.h \
    displayview.h \
    scopewidgetopgl.h \
    drawopglwidget.h \
    scope.h
#    widgetopengl.h

LIBS += -pthread
