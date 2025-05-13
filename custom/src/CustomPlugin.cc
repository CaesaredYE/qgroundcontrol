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
#include <QDebug>

CustomPlugin::CustomPlugin(QGCApplication *app, QGCToolbox *toolbox)
    : QGCCorePlugin(app, toolbox)
{
    qmlRegisterSingletonInstance("RadarReceiver", 1, 0, "RadarReceiver", new RadarReceiver(this));
}

void CustomPlugin::sendTargetLocation(Vehicle *vehicle, float lat, float lon, float alt)
{
     qDebug() << "lat:" << lat<< "lon:" << lon << "alt:" << alt;

    // vehicle->sendMavCommand(MAV_COMP_ID_UDP_BRIDGE,
    //                            MAV_CMD_USER_1,
    //                            false,
    //                            NAN,
    //                            NAN,
    //                            NAN,
    //                            NAN,
    //                            lat,
    //                            lon,
    //                            alt);
}

void CustomPlugin::sendPositionMsg(Vehicle *vehicle, const QString& x, const QString& y, const QString& z)
{
    qDebug() << "toDouble x:" << QString::number(x.toDouble(), 'f', 10);
    qDebug() << "toDouble y:" << QString::number(y.toDouble(), 'f', 10);
    qDebug() << "toFloat z:" << QString::number(z.toFloat());


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

void CustomPlugin::sendStopCommand(Vehicle *vehicle)
{
    vehicle->sendMavCommandInt(MAV_COMP_ID_UDP_BRIDGE,
                               MAV_CMD_USER_2,
                               MAV_FRAME_GLOBAL,
                               false,
                               NAN,
                               NAN,
                               NAN,
                               NAN,
                               NAN,
                               NAN,
                               NAN);
}
