QT       += core gui
QT       += charts
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GameSettings.cpp \
    SWIUtils.cpp \
    cinematicwidget.cpp \
    crisscrossprogressbar.cpp \
    doorstyleprogressbar.cpp \
    endgamewidget.cpp \
    endscorewidget.cpp \
    fancyprogressbarwrapper.cpp \
    gamewindow.cpp \
    gridprogressbar.cpp \
    introscreenwidget.cpp \
    main.cpp \
    maingamewidget.cpp \
    presentationwidget.cpp \
    scaledprogressbar.cpp \
    serverworker.cpp \
    spiralprogressbar.cpp \
    testinggraph.cpp \
    tiledprogressbar.cpp \
    updateworker.cpp \
    upgradebutton.cpp \
    windowshakeanimation.cpp \
    windowslideanimation.cpp

HEADERS += \
    IProgressBar.h \
    SWIUtils.h \
    autoclickerconfig.h \
    crisscrossprogressbar.h \
    doorstyleprogressbar.h \
    endgamewidget.h \
    endscorewidget.h \
    fancyprogressbarwrapper.h \
    gamesettings.h \
    gamewindow.h \
    gridprogressbar.h \
    introscreenwidget.h \
    presentationwidget.h \
    cinematicwidget.h \
    maingamewidget.h \
    scaledprogressbar.h \
    servergameplaystate.h \
    serverworker.h \
    spiralprogressbar.h \
    testinggraph.h \
    tiledprogressbar.h \
    updateworker.h \
    upgradebutton.h \
    windowshakeanimation.h \
    windowslideanimation.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
