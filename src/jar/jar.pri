load(qt_build_paths)
CONFIG += java
DESTDIR = $$MODULE_BASE_OUTDIR/jar

JAVACLASSPATH += $$PWD/src

JAVASOURCES += \
    $$PWD/src/com/cf/parse/CFAndroidParseController.java \
    $$PWD/src/com/cf/parse/CFAndroidParseBroadcastReceiver.java

JAVACLASSPATH += \
    $$PWD/libs/Parse-1.8.2.jar \
    $$PWD/libs/bolts-android-1.1.4.jar

# install
target.path = $$[QT_INSTALL_PREFIX]/jar
INSTALLS += target
