TEMPLATE = app
TARGET = dsquare
 
QT = core gui
 
HEADERS += dsquareConfig.h
SOURCES += dsquare.cpp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
