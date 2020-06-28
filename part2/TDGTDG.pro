#-------------------------------------------------
#
# Project created by QtCreator 2019-06-03T23:35:51
#
#-------------------------------------------------
DESTDIR = $$PWD/bin
QT       += core gui
QT       += multimedia
CONFIG += resources_big
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TDGTDG
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    bgmusic.cpp \
    bullet.cpp \
    cannon.cpp \
    cannonbullet.cpp \
    castle.cpp \
    enemy.cpp \
    gamewidgeta.cpp \
    judge.cpp \
        main.cpp \
    mainwidget.cpp \
    audioplayer.cpp \
    money.cpp \
    plistreader.cpp \
    tower.cpp \
    towerposition.cpp \
    waypoint.cpp
    bgmusic.cpp


HEADERS += \
    bullet.h \
    cannon.h \
    cannonbullet.h \
    castle.h \
    enemy.h \
    gamewidgeta.h \
    judge.h \
    mainwidget.h \
    audioplayer.h \
    bgmusic.h \
    money.h \
    plistreader.h \
    tower.h \
    towerposition.h \
    waypoint.h


FORMS += \
        gamewidgeta.ui \
        mainwidget.ui



RESOURCES += \
    file.qrc
