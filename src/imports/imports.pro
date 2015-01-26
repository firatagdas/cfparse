CXX_MODULE = cfparse
TARGET  = declarative_cfparse
TARGETPATH = CFParse
IMPORT_VERSION = 1.0

QT += qml quick cfparse cfparse-private
SOURCES += \
    $$PWD/cfparse.cpp

load(qml_plugin)

OTHER_FILES += qmldir
