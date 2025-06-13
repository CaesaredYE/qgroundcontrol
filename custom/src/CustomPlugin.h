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
#include "RadarController.h"
#include "RadarSettings.h"

class CustomPlugin : public QGCCorePlugin
{
    Q_OBJECT
    Q_PROPERTY(RadarController* radarController READ radarController CONSTANT)
    Q_PROPERTY(RadarSettings* radarSettings READ radarSettings CONSTANT)

public:
    CustomPlugin(QGCApplication *app, QGCToolbox *toolbox);

    void             setToolbox             (QGCToolbox* toolbox);
    QVariantList&    settingsPages          (void) final;

    RadarController* radarController        (void) { return _radarController; }
    RadarSettings*   radarSettings          (void) { return _radarSettings; }

    Q_INVOKABLE void sendTargetPosition     (const QString& lat, const QString& lon, const QString& alt);
    Q_INVOKABLE void emergencyStop          ();
    Q_INVOKABLE void sendUdpCommand         (const QString& ip, int port, const QString& message);

signals:
    void cmdOutput(const QString& cmd, const QString& output);

private slots:
    void onDataReceived                     ();

private:
    void _addSettingsEntry                  (const QString& title, const char* qmlFile, const char* iconFile = nullptr);

private:
    QVariantList        _customSettingsList;
    QUdpSocket*         _udpSocket = nullptr;

    RadarController*    _radarController;
    RadarSettings*      _radarSettings;
};
