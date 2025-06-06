import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles  1.4
import QtQuick.Layouts  1.2

import QGroundControl   1.0
import QGroundControl.Controls  1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FactControls  1.0
import QGroundControl.Palette       1.0

Rectangle {
    id:                 _root
    color:              qgcPal.window
    anchors.fill:       parent

    property var    radarController:    QGroundControl.corePlugin.radarController
    property var    radarSettings:      QGroundControl.corePlugin.radarSettings

    property real   _margins:           ScreenTools.defaultFontPixelWidth
    property real   _panelWidth:        _root.width * 0.6
    property real   _valueFieldWidth:   ScreenTools.defaultFontPixelWidth * 10

    QGCFlickable {
        clip:               true
        anchors.fill:       parent
        contentHeight:      settingsColumn.height
        contentWidth:       settingsColumn.width
        anchors.margins:    _margins

        Column {
            id:                     settingsColumn
            width:                  _root.width
            spacing:                _margins * 0.5

            Item {
                width:                      _panelWidth
                height:                     connectLabel.height
                anchors.margins:            _margins
                anchors.horizontalCenter:   parent.horizontalCenter
                QGCLabel {
                    id:             connectLabel
                    text:           qsTr("连接雷达")
                }
            }
            Rectangle {
                width:                      _panelWidth
                height:                     connectGrid.height + (_margins * 2)
                color:                      qgcPal.windowShade
                anchors.margins:            _margins
                anchors.horizontalCenter:   parent.horizontalCenter

                GridLayout {
                    id:                         connectGrid
                    anchors.horizontalCenter:   parent.horizontalCenter
                    anchors.topMargin:          _margins
                    anchors.top:                parent.top
                    columns:                    2

                    QGCLabel { text: qsTr("IP") }
                    FactTextField {
                        fact:                   radarSettings.ip
                        Layout.fillWidth:       true
                    }

                    QGCLabel { text: qsTr("Port") }
                    FactTextField {
                        fact:                   radarSettings.port
                        Layout.fillWidth:       true
                    }


                    QGCLabel { text: qsTr("本机Port") }
                    FactTextField {
                        fact:                  radarSettings.localPort
                        Layout.fillWidth:       true
                    }

                    FactCheckBox {
                        text:                   qsTr("自动连接")
                        fact:                   radarSettings.autoConnect
                        Layout.columnSpan:      2
                    }

                    Row {
                        Layout.alignment:           Qt.AlignHCenter
                        Layout.columnSpan:          2
                        spacing:                    _margins

                        QGCButton {
                            text:       qsTr("连接")
                            enabled:    !radarController.isConnected
                            onClicked:  radarController.connectRadar()
                        }

                        QGCButton {
                            text:       qsTr("断开")
                            enabled:    radarController.isConnected
                            onClicked:  radarController.disconnectRadar()
                        }
                    }
                }
            }


            Item { width: 1; height: _margins; }
            Item {
                width:                      _panelWidth
                height:                     positionLabel.height
                anchors.margins:            _margins
                anchors.horizontalCenter:   parent.horizontalCenter
                QGCLabel {
                    id:             positionLabel
                    text:           qsTr("雷达位置")
                }
            }
            Rectangle {
                width:                      _panelWidth
                height:                     positionGrid.height + (_margins * 2)
                color:                      qgcPal.windowShade
                anchors.margins:            _margins
                anchors.horizontalCenter:   parent.horizontalCenter

                GridLayout {
                    id:                         positionGrid
                    anchors.horizontalCenter:   parent.horizontalCenter
                    anchors.topMargin:          _margins
                    anchors.top:                parent.top
                    columns:                    2

                    QGCLabel { text: qsTr("纬度") }
                    FactTextField {
                        fact:                   radarSettings.latitude
                        Layout.fillWidth:       true
                    }

                    QGCLabel { text: qsTr("经度") }
                    FactTextField {
                        fact:                   radarSettings.longitude
                        Layout.fillWidth:       true
                    }
                }
            }
        }
    }
}
