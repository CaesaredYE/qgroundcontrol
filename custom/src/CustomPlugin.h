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
#include "RadarReceiver.h"
#include "RadarSettings.h"

class CustomPlugin : public QGCCorePlugin
{
    Q_OBJECT

public:
    CustomPlugin(QGCApplication *app, QGCToolbox *toolbox);

    QVariantList&    settingsPages                   (void) final;

    Q_INVOKABLE void connectRadar();
    Q_INVOKABLE void startSurvey();
    Q_INVOKABLE void stopSurvey();

    Q_INVOKABLE void sendLocationCmd(const QString& lat, const QString& lon, const QString& alt);
    Q_INVOKABLE void sendStopCmd();

private:
    void _addSettingsEntry(const QString& title, const char* qmlFile, const char* iconFile = nullptr);

private:
    QVariantList   _customSettingsList;

    RadarReceiver* _radarReceiver = nullptr;
    RadarSettings* _radarSettings = nullptr;
};
