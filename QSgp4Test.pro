#-------------------------------------------------
#
# Project created by QtCreator 2014-10-05T17:58:16
#
#-------------------------------------------------

QT       += core

QT       += gui

TARGET = QSgp4Test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

unix:INCLUDEPATH +=  ../QSgp4
else:win32:INCLUDEPATH += ../QSgp4

unix:!macx|win32: LIBS += -L$$PWD/../libs/linux_gplusplus -lqsgp4

OTHER_FILES += \
    SGP4-VER.TLE
