include($$PWD/cfparse-lib.pri)

load(qt_build_config)

TARGET = CFParse

QT += quick qml cfmvc cfmvc-private core-private

load(qt_module)

