QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gameboard.cpp \
    kingpiece.cpp \
    main.cpp \
    mainwindow.cpp \
    pieceprototype.cpp \
    player.cpp \
    powerup.cpp \
    regularpiece.cpp \
    rulespopup.cpp \
    tile.cpp \
    triplekingpiece.cpp \
    winnerpopup.cpp

HEADERS += \
    gameboard.h \
    kingpiece.h \
    mainwindow.h \
    pieceprototype.h \
    pieceprototypefactory.h \
    player.h \
    powerup.h \
    regularpiece.h \
    rulespopup.h \
    tile.h \
    triplekingpiece.h \
    winnerpopup.h

FORMS += \
    mainwindow.ui \
    rulespopup.ui \
    winnerpopup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
