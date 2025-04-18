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
#include "QGCApplication.h"

CustomPlugin::CustomPlugin(QGCApplication *app, QGCToolbox *toolbox)
    : QGCCorePlugin(app, toolbox)
{
}

void CustomPlugin::sendPositionMsg(float x, float y, float z)
{
    _activeVehicle->sendMavCommand(0, MAV_CMD_USER_1, false, x, y, z);
}

void CustomPlugin::sendStopCommand()
{
    _activeVehicle->sendMavCommand(0, MAV_CMD_USER_2, false);
}
