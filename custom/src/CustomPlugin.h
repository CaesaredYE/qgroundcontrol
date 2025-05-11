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
    ~CustomPlugin();

    RadarReceiver* radarReceiver() { return _radarReceiver; }

    Q_INVOKABLE void sendPositionMsg(Vehicle *vehicle, const QString& x, const QString& y, const QString& z);

    Q_INVOKABLE void sendStopCommand(Vehicle *vehicle);

private:
    RadarReceiver* _radarReceiver;
};
