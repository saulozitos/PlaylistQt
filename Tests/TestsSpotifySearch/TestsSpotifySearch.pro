QT += testlib network widgets
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_testspotifysearch.cpp \
    ../../PlaylistQt/auth.cpp \
    ../../PlaylistQt/searchengine.cpp \
    ../../PlaylistQt/settings.cpp \
    ../../PlaylistQt/spotify.cpp

HEADERS += \
    ../../PlaylistQt/auth.h \
    ../../PlaylistQt/musictrack.h \
    ../../PlaylistQt/searchengine.h \
    ../../PlaylistQt/settings.h \
    ../../PlaylistQt/spotify.h
