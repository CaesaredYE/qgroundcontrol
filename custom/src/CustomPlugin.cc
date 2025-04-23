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
}

CustomPlugin::~CustomPlugin()
{
}

void CustomPlugin::sendPositionMsg(Vehicle *vehicle, float x, float y, float z)
{
    vehicle->sendMavCommand(MAV_COMP_ID_UDP_BRIDGE,
                            MAV_CMD_USER_1,
                            false,
                            x,
                            y,
                            z,
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
