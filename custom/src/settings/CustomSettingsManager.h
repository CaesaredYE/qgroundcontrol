#pragma once

#include "SettingsManager.h"
#include "RadarSettings.h"

class CustomSettingsManager : public SettingsManager {
    Q_OBJECT
public:
    CustomSettingsManager(QObject* parent = nullptr);

    RadarSettings* radarSettings() { return _radarSettings; }

private:
    RadarSettings* _radarSettings;
};
