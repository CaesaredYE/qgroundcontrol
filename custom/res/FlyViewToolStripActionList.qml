/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQml.Models 2.12
import QtQuick.Layouts          1.11
import QGroundControl           1.0
import QGroundControl.Controls  1.0
import QGroundControl.ScreenTools   1.0

ToolStripActionList {
    id: _root

    signal displayPreFlightChecklist

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
            text:       qsTr("紧急停止")
            iconSource: "/res/CustomStop.svg"
            visible:    _guidedController.showLand && !_guidedController.showTakeoff
            enabled:    _guidedController.showLand

            onTriggered: {
                QGroundControl.corePlugin.sendStopCmd();
            }
        },
        ToolStripAction {
            text:       qsTr("发送坐标")
            iconSource: "/res/CustomLocation.svg"
            visible:    _guidedController.showTakeoff
            enabled:    _guidedController.showTakeoff

            onTriggered: {
                locationDialog.visible = true
            }
        }
    ]
}

QGCPopupDialog {
    id: locationDialog
    title: qsTr("敌机坐标")
    modal: true

    GridLayout {
        columnSpacing:  ScreenTools.defaultFontPixelWidth * 2
        rowSpacing:  ScreenTools.defaultFontPixelWidth * 2
        columns: 2

        QGCLabel { text: qsTr("纬度") }
        QGCTextField { id: lat}

        QGCLabel { text: qsTr("经度") }
        QGCTextField { id: lon }

        QGCLabel { text: qsTr("高度") }
        QGCTextField { id: alt }
    }

    QGCButton {
        text: qsTr("发送")
        enabled: _activeVehicle
        onClicked: {
            QGroundControl.corePlugin.sendLocationCmd(lat.text, lon.text, alt.text);
        }
    }
}

