TEMPLATE = app

QT       += testlib
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

TARGET = tst_qtinyaestest

QTAES_KEYSIZE = 128

include(../vendor/vendor.pri)
include(../de_skycoder42_qtinyaes.pri)

SOURCES += tst_qtinyaestest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
