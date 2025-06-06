#include "RadarSettings.h"

#include <QQmlEngine>

DECLARE_SETTINGGROUP(Radar, "Radar")
{
    qmlRegisterUncreatableType<RadarSettings>("QGroundControl", 1, 0, "RadarSettings", "Reference only");
}

DECLARE_SETTINGSFACT(RadarSettings, ip)
DECLARE_SETTINGSFACT(RadarSettings, port)
DECLARE_SETTINGSFACT(RadarSettings, localPort)
DECLARE_SETTINGSFACT(RadarSettings, latitude)
DECLARE_SETTINGSFACT(RadarSettings, longitude)
DECLARE_SETTINGSFACT(RadarSettings, autoConnect)
