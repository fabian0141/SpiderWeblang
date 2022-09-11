#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T20:37:41
#
#-------------------------------------------------

QT       += core gui
QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Spider
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    webpage/array.cpp \
    webpage/drawwidgetclass.cpp \
    webpage/environment.cpp \
    webpage/interpreter.cpp \
    webpage/loxclass.cpp \
    webpage/loxfunction.cpp \
    webpage/loxinstance.cpp \
    webpage/parser.cpp \
    webpage/resolver.cpp \
    webpage/scanner.cpp \
    webpage/token.cpp \
    webpage/webpage.cpp \
    webpage/widget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    webpage/array.h \
    webpage/drawwidgetclass.h \
    webpage/environment.h \
    webpage/expr.h \
    webpage/interpreter.h \
    webpage/loxcallable.h \
    webpage/loxclass.h \
    webpage/loxfunction.h \
    webpage/loxinstance.h \
    webpage/object.h \
    webpage/parser.h \
    webpage/resolver.h \
    webpage/return.h \
    webpage/scanner.h \
    webpage/stmt.h \
    webpage/style.h \
    webpage/token.h \
    webpage/tokentype.h \
    webpage/webpage.h \
    webpage/widget.h \
    mainwindow.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    lib/Spider/script \
    lib/Spider/script \
    lib/script \
    Spider.pro.user \
    lib/Spider/Widget
