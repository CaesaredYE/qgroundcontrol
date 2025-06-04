#pragma once

#include "SettingsGroup.h"

class RadarSettings : public SettingsGroup
{
    Q_OBJECT

public:
    RadarSettings(QObject* parent = nullptr);

    DEFINE_SETTING_NAME_GROUP()

    DEFINE_SETTINGFACT(radarIP)
    DEFINE_SETTINGFACT(radarPort)
    DEFINE_SETTINGFACT(radarLocalPort)
    DEFINE_SETTINGFACT(radarLatitude)
    DEFINE_SETTINGFACT(radarLongitude)
};
