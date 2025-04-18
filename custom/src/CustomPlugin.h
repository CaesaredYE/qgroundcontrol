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

class CustomPlugin;

class CustomPlugin : public QGCCorePlugin
{
    Q_OBJECT
public:
    CustomPlugin(QGCApplication *app, QGCToolbox *toolbox);
    ~CustomPlugin();

    Q_INVOKABLE void sendPositionMsg(float x = 0.0f, float y = 0.0f, float z = 0.0f);

    Q_INVOKABLE void sendStopCommand();
};
