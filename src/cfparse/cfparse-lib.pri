ANDROID_BUNDLED_JAR_DEPENDENCIES = \
    jar/CFAndroidParse-bundled.jar
ANDROID_JAR_DEPENDENCIES = \
    jar/CFAndroidParse.jar

INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
    $$PWD/cfparse_global.h \
    $$PWD/cfparsemodel.h

PRIVATE_HEADERS += \
    $$PWD/cfparsemodel_p.h

SOURCES += \
    $$PWD/cfparsemodel.cpp

android {
    QT += androidextras

    PRIVATE_HEADERS += \
        $$PWD/cfparsemodel_android_p.h

    SOURCES += \
        $$PWD/cfparsemodel_android_p.cpp

} else:ios {
    QMAKE_OBJECTIVE_CFLAGS += -F $$clean_path($$PWD/../libs) -framework Parse -framework Bolts

    PRIVATE_HEADERS += \
        $$PWD/cfparsemodel_ios_p.h

    OBJECTIVE_SOURCES += \
        $$PWD/cfparsemodel_ios_p.mm

} else {

    PRIVATE_HEADERS += \
        $$PWD/cfparsemodel_default_p.h

    SOURCES += \
        $$PWD/cfparsemodel_default_p.cpp
}


HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
