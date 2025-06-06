#pragma once

#include "SettingsGroup.h"

class RadarSettings : public SettingsGroup
{
    Q_OBJECT

public:
    RadarSettings(QObject* parent = nullptr);

    DEFINE_SETTING_NAME_GROUP()

    DEFINE_SETTINGFACT(ip)
    DEFINE_SETTINGFACT(port)
    DEFINE_SETTINGFACT(localPort)
    DEFINE_SETTINGFACT(latitude)
    DEFINE_SETTINGFACT(longitude)
    DEFINE_SETTINGFACT(autoConnect)
};
