/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

import QtLocation 5.3
import QtPositioning 5.3
import QtQuick.Window 2.2
import QtQml.Models 2.1

import QGroundControl 1.0
import QGroundControl.Controllers 1.0
import QGroundControl.Controls 1.0
import QGroundControl.FactSystem 1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap 1.0
import QGroundControl.Palette 1.0
import QGroundControl.ScreenTools 1.0
import QGroundControl.Vehicle 1.0

import MAVLink 1.0

Item {
    id: _root

    property var parentToolInsets               // These insets tell you what screen real estate is available for positioning the controls in your overlay
    property var totalToolInsets: _toolInsets // These are the insets for your custom overlay additions
    property var mapControl

    property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle

    QGCToolInsets {
        id: _toolInsets
        leftEdgeTopInset: parentToolInsets.leftEdgeTopInset
        leftEdgeCenterInset: parentToolInsets.leftEdgeCenterInset
        leftEdgeBottomInset: parentToolInsets.leftEdgeBottomInset
        rightEdgeTopInset: parentToolInsets.rightEdgeTopInset
        rightEdgeCenterInset: parentToolInsets.rightEdgeCenterInset
        rightEdgeBottomInset: parentToolInsets.rightEdgeBottomInset
        topEdgeLeftInset: parentToolInsets.topEdgeLeftInset
        topEdgeCenterInset: parentToolInsets.topEdgeCenterInset
        topEdgeRightInset: parentToolInsets.topEdgeRightInset
        bottomEdgeLeftInset: parentToolInsets.bottomEdgeLeftInset
        bottomEdgeCenterInset: parentToolInsets.bottomEdgeCenterInset
        bottomEdgeRightInset: parentToolInsets.bottomEdgeRightInset
    }

    Rectangle {
        id: sendRect
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: _toolsMargin + parentToolInsets.bottomEdgeRightInset
        anchors.rightMargin: _toolsMargin
        width: column.width
        height: column.height
        radius: 2
        color: qgcPal.window

        Column {
            id: column
            padding: _toolsMargin
            spacing: _toolsMargin

            QGCLabel {
                text: "敌机坐标"
                font.bold: true
            }

            Row {
                spacing: ScreenTools.defaultFontPixelWidth

                QGCLabel {
                    id: labelX
                    anchors.verticalCenter: inputX.verticalCenter
                    text: qsTr("x")
                }

                QGCTextField {
                    id: inputX
                    width: ScreenTools.defaultFontPixelHeight * 8
                    height: labelX.height * 1.2
                }
            }

            Row {
                spacing: ScreenTools.defaultFontPixelWidth

                QGCLabel {
                    id: labelY
                    anchors.verticalCenter: inputY.verticalCenter
                    text: qsTr("y")
                }

                QGCTextField {
                    id: inputY
                    width: ScreenTools.defaultFontPixelHeight * 8
                    height: labelY.height * 1.2
                }
            }

            Row {
                spacing: ScreenTools.defaultFontPixelWidth

                QGCLabel {
                    id: labelZ
                    anchors.verticalCenter: inputZ.verticalCenter
                    text: qsTr("z")
                }

                QGCTextField {
                    id: inputZ
                    width: ScreenTools.defaultFontPixelHeight * 8
                    height: labelZ.height * 1.2
                }
            }

            Row {
                spacing: ScreenTools.defaultFontPixelWidth
                anchors.horizontalCenter: parent.horizontalCenter

                QGCButton {
                    id: sendPosition
                    text: qsTr("发送")
                    enabled: _activeVehicle
                    onClicked: {
                        console.log("MAV_CMD_USER_1", inputX.text, inputY.text, inputZ.text);
                        QGroundControl.corePlugin.sendPositionMsg(_activeVehicle, inputX.text, inputY.text, inputZ.text);
                    }
                }

                QGCButton {
                    id: stop
                    text: qsTr("紧急停止")
                    enabled: _activeVehicle
                    onClicked: {
                        QGroundControl.corePlugin.sendStopCommand(_activeVehicle);
                    }
                }
            }
        }
    }
}
