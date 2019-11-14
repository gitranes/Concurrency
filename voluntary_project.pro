TEMPLATE = app
CONFIG += console c++1z
CONFIG += warn_on
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -pthread

QMAKE_CXXFLAGS += -Wall

SOURCES += \
        intersection.cpp \
        main.cpp \
        track.cpp \
        tracksystem.cpp

HEADERS += \
    intersection.hh \
    track.hh \
    tracksystem.hh
