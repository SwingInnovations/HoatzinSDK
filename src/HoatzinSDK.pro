#-------------------------------------------------
#
# Project created by QtCreator 2014-12-11T21:40:39
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HoatzinSDK
TEMPLATE = app

VERSION = 0.0.1
QMAKE_TARGET_COMPANY = Swing Innovations
QMAKE_TARGET_PRODUCT = Swing Tech One SDK
QMAKE_TARGET_DESCRIPTION = Swing Tech One toolset
QMAKE_TARGET_COPYRIGHT = LGPL v2.1

SOURCES += main.cpp\
        mainwindow.cpp\
        Widgets/codeeditor.cpp \
        Math/Vector.cpp \
    Widgets/sceneview.cpp \
    Widgets/outliner.cpp \
    Widgets/timeline.cpp

HEADERS  += mainwindow.h \
    Widgets/codeeditor.h \
    Math/Matrix.h \
    Math/Quaternion.h \
    Math/Vector.h \
    Widgets/sceneview.h \
    Widgets/outliner.h \
    Widgets/timeline.h \
    overloads.h
