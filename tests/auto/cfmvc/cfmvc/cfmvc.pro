CONFIG += testcase parallel_test
TARGET = tst_cfparse
osx:CONFIG -= app_bundle


QT += cfparse cfparse-private testlib
SOURCES += \
    tst_cfparse.cpp
