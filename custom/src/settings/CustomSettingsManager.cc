#include "CustomSettingsManager.h"

CustomSettingsManager::CustomSettingsManager(QObject* parent)
    : SettingsManager(parent),
    _radarSettings  (nullptr)
{

}

void CustomSettingsManager::setToolbox(QGCToolbox* toolbox)
{
    SettingsManager::setToolbox(toolbox);

    _radarSettings = new RadarSettings(this);
}
