TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        basehttpserver.cpp \
        httpserver.cpp \
        main.cpp \
        reply.cpp

HEADERS += \
    ../Common/json.hpp \
    basehttpserver.h \
    httpserver.h \
    reply.h
