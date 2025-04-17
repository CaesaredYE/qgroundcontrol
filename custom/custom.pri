# Build a single flight stack by disabling APM support
CONFIG  += QGC_DISABLE_APM_MAVLINK
CONFIG  += QGC_DISABLE_APM_PLUGIN 
CONFIG  += QGC_DISABLE_APM_PLUGIN_FACTORY

CONFIG  += installer

DEFINES -= APP_VERSION_STR=\"\\\"$$APP_VERSION_STR\\\"\"
DEFINES += APP_VERSION_STR=\"\\\"0.0.1\\\"\"

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

HEADERS += \
    $$PWD/src/CustomPlugin.h \

INCLUDEPATH += \
    $$PWD/src \
