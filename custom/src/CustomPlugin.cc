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

#include "CustomPlugin.h"

CustomPlugin::CustomPlugin(QGCApplication *app, QGCToolbox *toolbox)
    : QGCCorePlugin(app, toolbox)
{
    _radarReceiver = new RadarReceiver(this);
    _radarReceiver->start(); 
    qmlRegisterSingletonInstance("RadarReceiver", 1, 0, "RadarReceiver", _radarReceiver);
}

void CustomPlugin::sendPositionCmd(Vehicle *vehicle, const QString& x, const QString& y, const QString& z)
{
    vehicle->sendMavCommandInt(MAV_COMP_ID_UDP_BRIDGE,
                               MAV_CMD_USER_1,
                               MAV_FRAME_GLOBAL,
                               false,
                               NAN,
                               NAN,
                               NAN,
                               NAN,
                               x.toDouble(),
                               y.toDouble(),
                               z.toFloat());
}

void CustomPlugin::sendStopCmd(Vehicle *vehicle)
{
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
