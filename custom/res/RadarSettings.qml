/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick          2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs  1.2
import QtQuick.Layouts  1.2

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Palette       1.0

Rectangle {
    id:                 _radarRoot
    color:              qgcPal.window
    anchors.fill:       parent
    anchors.margins:    ScreenTools.defaultFontPixelWidth

    QGCPalette {
        id:                 qgcPal
        colorGroupEnabled:  enabled
    }

    QGCFlickable {
        clip:               true
        anchors.top:        parent.top
        width:              parent.width
        height:             parent.height - buttonRow.height
        contentHeight:      settingsColumn.height
        contentWidth:       _radarRoot.width
        flickableDirection: Flickable.VerticalFlick

        Column {
            id:                 settingsColumn
            width:              _radarRoot.width
            anchors.margins:    ScreenTools.defaultFontPixelWidth
            spacing:            ScreenTools.defaultFontPixelHeight

            GridLayout {
                columns:        2
                columnSpacing:  ScreenTools.defaultFontPixelHeight
                rowSpacing:     ScreenTools.defaultFontPixelHeight

                QGCLabel { text: qsTr("Name") }
                QGCTextField {
                    id:                     nameField
                    Layout.preferredWidth:  _secondColumnWidth
                    Layout.fillWidth:       true
                    text:                   editingConfig.name
                    placeholderText:        qsTr("Enter name")
                }

                QGCLabel { text: qsTr("Name") }
                QGCTextField {
                    id:                     nameField
                    Layout.preferredWidth:  _secondColumnWidth
                    Layout.fillWidth:       true
                    text:                   editingConfig.name
                    placeholderText:        qsTr("Enter name")
                }

                QGCLabel { text: qsTr("Name") }
                QGCTextField {
                    id:                     nameField
                    Layout.preferredWidth:  _secondColumnWidth
                    Layout.fillWidth:       true
                    text:                   editingConfig.name
                    placeholderText:        qsTr("Enter name")
                }

                QGCLabel { text: qsTr("Name") }
                QGCTextField {
                    id:                     nameField
                    Layout.preferredWidth:  _secondColumnWidth
                    Layout.fillWidth:       true
                    text:                   editingConfig.name
                    placeholderText:        qsTr("Enter name")
                }

                QGCLabel { text: qsTr("Name") }
                QGCTextField {
                    id:                     nameField
                    Layout.preferredWidth:  _secondColumnWidth
                    Layout.fillWidth:       true
                    text:                   editingConfig.name
                    placeholderText:        qsTr("Enter name")
                }

            }
        }
    }

    Row {
        id:                 buttonRow
        spacing:            ScreenTools.defaultFontPixelWidth
        anchors.bottom:     parent.bottom
        anchors.margins:    ScreenTools.defaultFontPixelWidth
        anchors.horizontalCenter: parent.horizontalCenter

        QGCButton {
            text:       qsTr("连接")
            onClicked:  QGroundControl.linkManager.createConnectedLink()
        }
        QGCButton {
            text:       qsTr("取消连接")
            onClicked:  {}
        }
    }
}
