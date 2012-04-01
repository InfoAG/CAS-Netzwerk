TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += console

QT += network

HEADERS += ChatterBoxServer.h CAS.h lexer.h
SOURCES += ChatterBoxServer.cpp main.cpp CAS.cpp lexer.cpp
