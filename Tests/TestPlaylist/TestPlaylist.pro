QT += testlib multimedia
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_testplaylistclass.cpp \
    ../../PlaylistQt/playlist.cpp \
    ../../PlaylistQt/settings.cpp

HEADERS += \
    ../../PlaylistQt/playlist.h \
    ../../PlaylistQt/settings.h
