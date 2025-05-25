/****************************************************************************
 *
 * (c) 2009-2019 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 *   @brief Custom QGCCorePlugin Implementation
 *   @author Gus Grubba <gus@auterion.com>
 */
#include <QtQml>
#include <QQmlEngine>

#include "CustomPlugin.h"
#include "QmlComponentInfo.h"

CustomPlugin::CustomPlugin(QGCApplication *app, QGCToolbox *toolbox)
    : QGCCorePlugin(app, toolbox)
{
    _radarReceiver = new RadarReceiver(this);
    qmlRegisterSingletonInstance("RadarReceiver", 1, 0, "RadarReceiver", _radarReceiver);
    _radarReceiver->start();
    // _radarSettings = new RadarSettings(this);
    // qmlRegisterSingletonInstance("Radar.Settings", 1, 0, "RadarSettings", _radarSettings);
}

void CustomPlugin::_addSettingsEntry(const QString& title, const char* qmlFile, const char* iconFile)
{
    Q_CHECK_PTR(qmlFile);
    _customSettingsList.append(QVariant::fromValue(
        new QmlComponentInfo(title,
                             QUrl::fromUserInput(qmlFile),
                             iconFile == nullptr ? QUrl() : QUrl::fromUserInput(iconFile),
                             this)));
}

QVariantList& CustomPlugin::settingsPages()
{
    if(_customSettingsList.isEmpty()) {
        _addSettingsEntry(tr("General"),     "qrc:/qml/GeneralSettings.qml",     "qrc:/res/gear-white.svg");
        _addSettingsEntry(tr("Comm Links"),  "qrc:/qml/LinkSettings.qml",        "qrc:/res/waves.svg");
        _addSettingsEntry(tr("Offline Maps"),"qrc:/qml/OfflineMap.qml",          "qrc:/res/waves.svg");
        _addSettingsEntry(tr("MAVLink"),     "qrc:/qml/MavlinkSettings.qml",     "qrc:/res/waves.svg");
        _addSettingsEntry(tr("Console"),     "qrc:/qml/QGroundControl/Controls/AppMessages.qml");
        _addSettingsEntry(tr("Radar"),       "qrc:/qml/RadarSettings.qml");
    }
    return _customSettingsList;
}

void CustomPlugin::connectRadar()
{
    _radarReceiver->start();
}

void CustomPlugin::startSurvey()
{
    const uint8_t data[] = {
        0xaa, 0xaa, 0xaa, 0xaa,
        0x0b, 0x00, 0x00, 0x00,
        0x15, 0x01, 0x00, 0xff,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0xff, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00
    };
    _radarReceiver->writeData(data, sizeof(data));
}

void CustomPlugin::stopSurvey()
{
    const uint8_t data[] = {
        0xaa, 0xaa, 0xaa, 0xaa,
        0x0b, 0x00, 0x00, 0x00,
        0x15, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00
    };
    _radarReceiver->writeData(data, sizeof(data));
}

void CustomPlugin::sendLocationCmd(const QString& lat, const QString& lon, const QString& alt)
{
    Vehicle* vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    vehicle->sendMavCommandInt(MAV_COMP_ID_UDP_BRIDGE,
                               MAV_CMD_USER_1,
                               MAV_FRAME_GLOBAL,
                               false,
                               NAN,
                               NAN,
                               NAN,
                               NAN,
                               lat.toDouble(),
                               lon.toDouble(),
                               alt.toFloat());
}

void CustomPlugin::sendStopCmd()
{
    Vehicle* vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    vehicle->sendMavCommand(MAV_COMP_ID_UDP_BRIDGE,
                            MAV_CMD_USER_2,
                            false,
                            NAN,
                            NAN,
                            NAN,
                            NAN,
                            NAN,
                            NAN,
                            NAN);
}
