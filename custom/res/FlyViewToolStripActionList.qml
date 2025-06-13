/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQml.Models 2.12
import QGroundControl           1.0
import QGroundControl.Controls  1.0

ToolStripActionList {
    id: _root

    signal displayPreFlightChecklist
    signal showLocationDialog
    signal showStartupDialog

    property var    radarController:    QGroundControl.corePlugin.radarController

    model: [
        ToolStripAction {
            text:           qsTr("Plan")
            iconSource:     "/qmlimages/Plan.svg"
            onTriggered:    mainWindow.showPlanView()
        },
        PreFlightCheckListShowAction { onTriggered: displayPreFlightChecklist() },
        GuidedActionTakeoff { },
        GuidedActionLand { },
        GuidedActionRTL { },
        GuidedActionPause { },
        GuidedActionActionList { },
        GuidedActionGripper { },
        ToolStripAction {
            text:       qsTr("启动")
            iconSource: "/custom/img/terminal.svg"
            onTriggered: showStartupDialog()
        },
        ToolStripAction {
            text:       qsTr("紧急停止")
            iconSource: "/custom/img/stop.svg"
            visible:    _guidedController.showLand && !_guidedController.showTakeoff
            enabled:    _guidedController.showLand
            onTriggered: QGroundControl.corePlugin.emergencyStop()
        },
        ToolStripAction {
            text:       qsTr("发送坐标")
            iconSource: "/custom/img/location.svg"
            visible:    _guidedController._activeVehicle
            enabled:    _guidedController._activeVehicle
            onTriggered: showLocationDialog()
        },
        ToolStripAction {
            text:       qsTr("开启探测")
            iconSource: "/custom/img/radar.svg"
            visible:    radarController.isConnected
            enabled:    !radarController.isScanning
            onTriggered: radarController.startScan()
        },
        ToolStripAction {
            text:       qsTr("停止探测")
            iconSource: "/custom/img/stop.svg"
            visible:    radarController.isConnected
            enabled:    radarController.isScanning
            onTriggered: radarController.stopScan()
        }
    ]
}
