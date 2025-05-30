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
        0x00, 0x00, 0x00, 0x00
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
        0x00, 0x00, 0x00, 0x00
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
