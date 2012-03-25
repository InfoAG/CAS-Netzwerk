TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += console

QT += network

HEADERS += ChatterBoxServer.h CAS.h Lexer.h
SOURCES += ChatterBoxServer.cpp main.cpp CAS.cpp Lexer.cpp
