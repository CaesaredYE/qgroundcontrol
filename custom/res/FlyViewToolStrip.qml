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
            _root.locationDialog.visible = true
        }
    }

    model: flyViewToolStripActionList.model

    QGCPopupDialog {
        id: locationDialog
        title: qsTr("敌机坐标")
        modal: true
        visible: false

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
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: ScreenTools.defaultFontPixelWidth

                QGCButton {
                    text: qsTr("取消")
                    onClicked: locationDialog.visible = false
                }

                QGCButton {
                    text: qsTr("发送")
                    onClicked: {
                        QGroundControl.corePlugin.sendLocationCmd(lat.text, lon.text, alt.text);
                    }
                }
            }
        }
    }
}
