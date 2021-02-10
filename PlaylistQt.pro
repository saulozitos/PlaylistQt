QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    auth.cpp \
    main.cpp \
    playlist.cpp \
    searchformatter.cpp \
    settings.cpp \
    spotify.cpp \
    stackedspotify.cpp \
    uilogin.cpp \
    uiplayer.cpp

HEADERS += \
    auth.h \
    playlist.h \
    searchformatter.h \
    settings.h \
    spotify.h \
    stackedspotify.h \
    structs.h \
    uilogin.h \
    uiplayer.h

FORMS += \
    stackedspotify.ui \
    uilogin.ui \
    uiplayer.ui
    
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
