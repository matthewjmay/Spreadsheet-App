######################################################################
# Automatically generated by qmake (3.0) Wed May 3 21:18:12 2017
######################################################################

TEMPLATE = app
TARGET = SpreadsheetApp
INCLUDEPATH += .
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
# Input
HEADERS += finddialog.h gotocell.h mainwindow.h sortdialog.h \
    spreadsheet.h
FORMS += finddialog.ui gotocell.ui sortdialog.ui 
SOURCES += finddialog.cpp gotocell.cpp main.cpp mainwindow.cpp sortdialog.cpp \
    spreadsheet.cpp
CONFIG += release
