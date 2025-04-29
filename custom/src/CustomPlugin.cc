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
}

CustomPlugin::~CustomPlugin()
{
}

void CustomPlugin::sendPositionMsg(Vehicle *vehicle, const QString& x, const QString& y, const QString& z)
{

    qDebug() << "Variable x:" << x;
    qDebug() << "Variable y:" << y;
    qDebug() << "Variable z:" << z;

    qDebug() << "Variable x:" << x.toFloat();
    qDebug() << "Variable y:" << y.toFloat();
    qDebug() << "Variable z:" << z.toFloat();

    vehicle->sendMavCommand(MAV_COMP_ID_UDP_BRIDGE,
                            MAV_CMD_USER_1,
                            true,
                            x.toFloat(),
                            y.toFloat(),
                            z.toFloat(),
                            NAN,
                            NAN,
                            NAN,
                            NAN);

    vehicle->sendMavCommand(MAV_COMP_ID_UDP_BRIDGE,
                            MAV_CMD_USER_1,
                            true,
                            3.12456789f,
                            4.987654321f,
                            5.123456f,
                            NAN,
                            NAN,
                            NAN,
                            NAN);
}

void CustomPlugin::sendStopCommand(Vehicle *vehicle)
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
