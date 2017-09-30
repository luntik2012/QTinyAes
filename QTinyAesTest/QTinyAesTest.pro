TEMPLATE = app

QT       += testlib
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

TARGET = tst_qtinyaestest

SOURCES += tst_qtinyaestest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_/..) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)):include($$OUT_PWD/qpmx_generated.pri)
else: error(qpmx initialization failed. Check the compilation log for details.)

include(../de_skycoder42_qtinyaes.pri)
