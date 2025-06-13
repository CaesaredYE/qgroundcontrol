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
        onShowStartupDialog: {
            startup.createObject(mainWindow).open()
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

    Component {
        id: startup

        QGCPopupDialog {
            title: qsTr("启动")
            buttons:    StandardButton.Close

            property var outputs: ({
                                       ros: [],
                                       imageDetect: [],
                                       intercept: [],
                                       check: [],
                                   })

            Connections {
                target: QGroundControl.corePlugin
                function onCmdOutput(cmd, output) {
                    appendOutput(cmd, output)
                }
            }

            function startModule(cmd) {
                let message = {
                    type: "exec",
                    cmd
                }
                QGroundControl.corePlugin.sendUdpCommand(ipField.text, parseInt(portField.text), JSON.stringify(message))
                outputs[cmd] = []
                updateOutputs()
            }

            function updateOutputs() {
                rosOutput.text = outputs.ros.join("\n")
                imageDetectOutput.text = outputs.imageDetect.join("\n")
                interceptOutput.text = outputs.intercept.join("\n")
                checkOutput.text = outputs.check.join("\n")
            }

            function appendOutput(cmd, line) {
                outputs[cmd].push(line)
                updateOutputs()
            }

            ColumnLayout {
                spacing: ScreenTools.defaultFontPixelHeight

                GridLayout {
                    columns: 2
                    QGCLabel { text: qsTr("IP 地址") }
                    QGCTextField { id: ipField; text: "" }

                    QGCLabel { text: qsTr("端口") }
                    QGCTextField { id: portField; text: "6000" }
                }

                // 启动按钮
                RowLayout {
                    spacing: ScreenTools.defaultFontPixelWidth

                    QGCButton {
                        text: qsTr("ROS")
                        enabled: ipField.text && portField.text
                        onClicked: startModule("ros")
                    }

                    QGCButton {
                        text: qsTr("图像检测")
                        enabled: ipField.text && portField.text
                        onClicked: startModule("imageDetect")
                    }

                    QGCButton {
                        text: qsTr("拦截主程序")
                        enabled: ipField.text && portField.text
                        onClicked: startModule("intercept")
                    }

                    QGCButton {
                        text: qsTr("飞机自检")
                        enabled: ipField.text && portField.text
                        onClicked: startModule("check")
                    }
                }

                Rectangle {
                    color: "#222"; radius: 4; border.color: "#888"
                    Layout.fillWidth: true; height: 100

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 4

                        QGCLabel { text: qsTr(" ROS输出"); font.bold: true; color: "white" }
                        Flickable {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            contentHeight: rosOutput.contentHeight
                            clip: true

                            QGCLabel {
                                id: rosOutput
                                wrapMode: Text.Wrap
                            }
                        }
                    }
                }

                Rectangle {
                    color: "#222"; radius: 4; border.color: "#888"
                    Layout.fillWidth: true; height: 100

                    ColumnLayout {
                        anchors.fill: parent; spacing: 4
                        
                        QGCLabel { text: qsTr(" 图像检测输出"); font.bold: true; color: "white" }
                        Flickable {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            contentHeight: imageDetectOutput.contentHeight
                            clip: true

                            QGCLabel {
                                id: imageDetectOutput
                                wrapMode: Text.Wrap
                            }
                        }
                    }
                }

                Rectangle {
                    color: "#222"; radius: 4; border.color: "#888"
                    Layout.fillWidth: true; height: 100

                    ColumnLayout {
                        anchors.fill: parent; spacing: 4

                        QGCLabel { text: qsTr(" 拦截主程序输出"); font.bold: true; color: "white" }
                        Flickable {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            contentHeight: interceptOutput.contentHeight
                            clip: true

                            QGCLabel {
                                id: interceptOutput
                                wrapMode: Text.Wrap
                            }
                        }
                    }
                }

                Rectangle {
                    color: "#222"; radius: 4; border.color: "#888"
                    Layout.fillWidth: true; height: 100

                    ColumnLayout {
                        anchors.fill: parent; spacing: 4

                        QGCLabel { text: qsTr(" 飞机自检输出"); font.bold: true; color: "white" }
                        Flickable {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            contentHeight: checkOutput.contentHeight
                            clip: true

                            QGCLabel {
                                id: checkOutput
                                wrapMode: Text.Wrap
                            }
                        }
                    }
                }
            }
        }
    }
}
