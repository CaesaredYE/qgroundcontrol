#include "RadarSettings.h"

#include <QQmlEngine>
#include <QtQml>

DECLARE_SETTINGGROUP(Radar, "Radar")
{
    qmlRegisterUncreatableType<RadarSettings>("Radar.Settings", 1, 0, "RadarSettings", "Only accessible as a singleton");
}

DECLARE_SETTINGSFACT(RadarSettings, radarIP)
DECLARE_SETTINGSFACT(RadarSettings, radarPort)
DECLARE_SETTINGSFACT(RadarSettings, radarLocalPort)
DECLARE_SETTINGSFACT(RadarSettings, radarLatitude)
DECLARE_SETTINGSFACT(RadarSettings, radarLongitude)
