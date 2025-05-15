/****************************************************************************
 *
 * (c) 2009-2019 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 *   @brief Custom QGCCorePlugin Declaration
 *   @author Gus Grubba <gus@auterion.com>
 */

#pragma once

#include "QGCCorePlugin.h"
#include "Vehicle.h"
#include "RadarReceiver.h"

class CustomPlugin : public QGCCorePlugin
{
    Q_OBJECT

public:
    CustomPlugin(QGCApplication *app, QGCToolbox *toolbox);

    QVariantList&    settingsPages                   (void) final;

    Q_INVOKABLE void sendLocationCmd(const QString& lat, const QString& lon, const QString& alt);

    Q_INVOKABLE void sendStopCmd();

private:
    void _addSettingsEntry(const QString& title, const char* qmlFile, const char* iconFile = nullptr);

private:
    RadarReceiver* _radarReceiver = nullptr;
    QVariantList    _customSettingsList;
};
