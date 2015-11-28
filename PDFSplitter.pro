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


unix:!macx: LIBS += -L$$PWD/../../../pdf_hummus/Build/PDFWriter/ -lPDFWriter

INCLUDEPATH += $$PWD/../../../pdf_hummus/PDFWriter
DEPENDPATH += $$PWD/../../../pdf_hummus/PDFWriter

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../pdf_hummus/Build/PDFWriter/libPDFWriter.a

unix:!macx: LIBS += -L$$PWD/../../../pdf_hummus/Build/FreeType/ -lFreeType

INCLUDEPATH += $$PWD/../../../pdf_hummus/FreeType/include
DEPENDPATH += $$PWD/../../../pdf_hummus/FreeType/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../pdf_hummus/Build/FreeType/libFreeType.a

unix:!macx: LIBS += -L$$PWD/../../../pdf_hummus/Build/LibJpeg/ -lLibJpeg

INCLUDEPATH += $$PWD/../../../pdf_hummus/LibJpeg
DEPENDPATH += $$PWD/../../../pdf_hummus/LibJpeg

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../pdf_hummus/Build/LibJpeg/libLibJpeg.a

unix:!macx: LIBS += -L$$PWD/../../../pdf_hummus/Build/LibTiff/ -lLibTiff

INCLUDEPATH += $$PWD/../../../pdf_hummus/LibTiff
DEPENDPATH += $$PWD/../../../pdf_hummus/LibTiff

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../pdf_hummus/Build/LibTiff/libLibTiff.a

unix:!macx: LIBS += -L$$PWD/../../../pdf_hummus/Build/ZLib/ -lZlib

INCLUDEPATH += $$PWD/../../../pdf_hummus/ZLib
DEPENDPATH += $$PWD/../../../pdf_hummus/ZLib

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../pdf_hummus/Build/ZLib/libZlib.a
