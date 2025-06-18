

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
import QGroundControl.FlightDisplay 1.0
import QGroundControl.Palette 1.0
import QGroundControl.ScreenTools 1.0

Item {
    property var parentToolInsets
    // These insets tell you what screen real estate is available for positioning the controls in your overlay
    property var totalToolInsets: _toolInsets // These are the insets for your custom overlay additions
    property var mapControl
    property real _margins:           ScreenTools.defaultFontPixelWidth * 0.75

    property var radarController: QGroundControl.corePlugin.radarController
    property var isPlane: false

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
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: _margins + parentToolInsets.bottomEdgeRightInset
        anchors.rightMargin: _margins
        radius: 4
        color: qgcPal.window
        width: radio.implicitWidth + 24
        height: radio.implicitHeight + 16
        visible: radarController.isScanning


        QGCCheckBox {
            id: radio
            anchors.centerIn: parent
            text: qsTr("目标是否是飞机")
            checked: isPlane
            onClicked: {
                isPlane = !isPlane
                radarController.confirmTarget()
            }
        }
    }
}
