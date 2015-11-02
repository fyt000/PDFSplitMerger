#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T21:32:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#QT += widgets

TARGET = PDFSplitter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pdfcore.cpp

HEADERS  += mainwindow.h \
    pdfcore.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lpodofo

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include
