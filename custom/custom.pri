# Build a single flight stack by disabling APM support
CONFIG  += QGC_DISABLE_APM_MAVLINK
CONFIG  += QGC_DISABLE_APM_PLUGIN 
CONFIG  += QGC_DISABLE_APM_PLUGIN_FACTORY

CUSTOM_QGC_VERSION = "0.0.1"

DEFINES -= APP_VERSION_STR=\"\\\"$$APP_VERSION_STR\\\"\"
DEFINES += APP_VERSION_STR=\"\\\"$$CUSTOM_QGC_VERSION\\\"\"

DEFINES += CUSTOMHEADER=\"\\\"CustomPlugin.h\\\"\"
DEFINES += CUSTOMCLASS=CustomPlugin

TARGET   = DropStrike
DEFINES += QGC_APPLICATION_NAME='"\\\"DropStrike\\\""'

DEFINES += QGC_ORG_NAME=\"\\\"polixir\\\"\"
DEFINES += QGC_ORG_DOMAIN=\"\\\"polixir.ai\\\"\"

QGC_APP_NAME        = "DropStrike"
QGC_BINARY_NAME     = "DropStrike"
QGC_ORG_NAME        = "polixir"
QGC_ORG_DOMAIN      = "polixir.ai"
QGC_APP_DESCRIPTION = "DropStrike Ground Control Station"

RESOURCES += \
    $$PWD/custom.qrc

QML_IMPORT_PATH += \
    $$PWD/res

SOURCES += \
    $$PWD/src/CustomPlugin.cc \
    $$PWD/src/radar/RadarController.cc \
    $$PWD/src/radar/RadarSettings.cc \

HEADERS += \
    $$PWD/src/CustomPlugin.h \
    $$PWD/src/radar/RadarController.h \
    $$PWD/src/radar/RadarSettings.h \

INCLUDEPATH += \
    $$PWD/src \
    $$PWD/src/radar \

MacBuild {
    ICON = $$PWD/res/icons/macx.icns
}

WindowsBuild {
    RC_ICONS = $$PWD/res/icons/dropstrike.ico
}
