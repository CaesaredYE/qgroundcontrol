/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.11
import QtQuick.Layouts          1.11
import QtQuick.Dialogs          1.2

import QtQml.Models 2.12

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.FlightDisplay 1.0

ToolStrip {
    id:     _root
    title:  qsTr("Fly")

    signal displayPreFlightChecklist

    FlyViewToolStripActionList {
        id: flyViewToolStripActionList

        onDisplayPreFlightChecklist: _root.displayPreFlightChecklist()
        onShowLocationDialog: {
            sendLocation.createObject(mainWindow).open()
        }
    }

    model: flyViewToolStripActionList.model

    Component {
        id: sendLocation

        QGCPopupDialog {
            title: qsTr("敌机坐标")
            buttons:    StandardButton.Close

            ColumnLayout {
                GridLayout {
                    columnSpacing:  ScreenTools.defaultFontPixelWidth
                    rowSpacing:  ScreenTools.defaultFontPixelWidth
                    columns: 2

                    QGCLabel { text: qsTr("纬度") }
                    QGCTextField { id: lat}

                    QGCLabel { text: qsTr("经度") }
                    QGCTextField { id: lon }

                    QGCLabel { text: qsTr("高度") }
                    QGCTextField { id: alt }

                    QGCButton {
                        text: qsTr("发送")
                        Layout.alignment:   Qt.AlignRight
                        Layout.columnSpan:  2
                        onClicked: {
                            QGroundControl.corePlugin.sendTargetPosition(lat.text, lon.text, alt.text);
                        }
                    }
                }
            }
        }
    }
}
