TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        basehttpserver.cpp \
        httpserver.cpp \
        main.cpp

HEADERS += \
    basehttpserver.h \
    httpserver.h
