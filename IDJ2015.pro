TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += src/game.cpp \
    src/main.cpp \
    src/state.cpp

LIBPATH += C:\SDL\lib

LIBS += -lMingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDEPATH += C:\SDL\include\SDL2
include(deployment.pri)
qtcAddDeployment()

HEADERS += include/game.h \
    include/state.h

