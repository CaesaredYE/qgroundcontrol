/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                      2.11
import QtQuick.Controls             2.4
import QtLocation                   5.3
import QtPositioning                5.3
import QtQuick.Dialogs              1.2
import QtQuick.Layouts              1.11

import QGroundControl               1.0
import QGroundControl.Controllers   1.0
import QGroundControl.Controls      1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Vehicle       1.0

Item {
    id: radar
    property var    map
    property bool   largeMapView

    property var    radarController:    QGroundControl.corePlugin.radarController
    property var    radarSettings:      QGroundControl.corePlugin.radarSettings
    property var    mapCenter:          undefined

    Component.onCompleted: updateMapCenter()

    Connections {
        target: radarSettings.latitude
        onValueChanged: updateMapCenter()
    }
    Connections {
        target: radarSettings.longitude
        onValueChanged: updateMapCenter()
    }

    function updateMapCenter() {
        if (radarSettings.latitude.value && radarSettings.longitude.value) {
            mapCenter = QtPositioning.coordinate(
                        radarSettings.latitude.value,
                        radarSettings.longitude.value
                        )

            if (map) {
                map.center = mapCenter
            }
        }
    }

    // 目标列表
    MapItemView {
        id: mapItemView
        parent: map
        model: radarController.trackList
        z: QGroundControl.zOrderMapItems + 1
        visible: radarController.isScanning
        delegate: MapQuickItem {
            parent: map
            visible:    modelData.existFlag === 1
            coordinate: QtPositioning.coordinate(modelData.lat, modelData.lon)
            anchorPoint.x: targetRect.width / 2
            anchorPoint.y: targetRect.width / 2
            z: QGroundControl.zOrderMapItems + 1

            sourceItem: Item {
                Rectangle {
                    id: targetRect
                    width: 20
                    height: 20
                    radius: targetRect.width / 2
                    color: "red"
                    border.color: "black"
                    border.width: 1
                    anchors.centerIn: parent

                    MouseArea {
                        id: targetMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }

                Rectangle {
                    visible: targetMouseArea.containsMouse || tooltipMouseArea.containsMouse
                    anchors.horizontalCenter: targetRect.horizontalCenter
                    anchors.top: targetRect.bottom
                    anchors.topMargin: 5
                    color: qgcPal.window
                    radius: 4
                    border.color: "black"
                    border.width: 1
                    width: column.width
                    height: column.height

                    Column {
                        id: column
                        padding: 10
                        spacing: 5
                        anchors.margins: 5
                        anchors.fill: parent

                        Text {
                            color: "white"
                            text: "方位: " + modelData.compass
                        }
                        Text {
                            color: "white"
                            text: "距离: " + modelData.distance
                        }
                        Text {
                            color: "white"
                            text: "航向: " + modelData.course
                        }
                        Text {
                            color: "white"
                            text: "航速: " + modelData.speed
                        }
                        Text {
                            color: "white"
                            text: "纬度: " + modelData.lat
                        }
                        Text {
                            color: "white"
                            text: "经度: " + modelData.lon
                        }
                        Text {
                            color: "white"
                            text: "高度: " + modelData.alt
                        }
                        QGCButton {
                            text: "选为目标"
                            onClicked: {
                                radarController.setTargetBatch(modelData.batch)
                            }
                        }
                    }

                    MouseArea {
                        id: tooltipMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }
            }
        }
    }

    // 遮罩
    MapQuickItem {
        parent: map
        coordinate: map.center
        anchorPoint.x: map.width / 2
        anchorPoint.y: map.height / 2
        z: QGroundControl.zOrderMapItems - 1
        visible: radarController.isScanning
        sourceItem: Rectangle {
            width: map.width
            height: map.height
            color: Qt.rgba(0, 0, 0, 0.5)
        }
    }

    // 雷达中心点
    MapQuickItem {
        parent: map
        coordinate: mapCenter
        anchorPoint.x: radarScan.width / 2
        anchorPoint.y: radarScan.height / 2
        visible: radarController.isScanning
        z: QGroundControl.zOrderMapItems
        sourceItem: radarScan
    }

    // 雷达扫描效果
    Item {
        id: radarScan
        width: 1024
        height: 1024
        
        Canvas {
            id: radarCanvas
            anchors.fill: parent
            property real sweepAngle: 30
            property real rotationAngle: 0

            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);

                ctx.save();
                ctx.translate(width / 2, height / 2);

                // === 1. 绘制同心圆 ===
                ctx.strokeStyle = "rgba(255, 0, 0, 0.9)";
                ctx.lineWidth = 1;
                var ringCount = 4; // 同心圆数量
                var maxRadius = width / 2;
                for (var i = 1; i <= ringCount; i++) {
                    var radius = (i / ringCount) * maxRadius;
                    ctx.beginPath();
                    ctx.arc(0, 0, radius, 0, 2 * Math.PI);
                    ctx.stroke();
                }

                // === 2. 绘制旋转扇形 ===
                ctx.rotate(rotationAngle * Math.PI / 180);

                var gradient = ctx.createRadialGradient(0, 0, 0, 0, 0, maxRadius);
                gradient.addColorStop(0, "rgba(0, 255, 0, 0.7)");
                gradient.addColorStop(1, "rgba(0, 255, 0, 0)");

                ctx.beginPath();
                ctx.moveTo(0, 0);
                ctx.arc(0, 0, maxRadius, 0, sweepAngle * Math.PI / 180);
                ctx.closePath();
                ctx.fillStyle = gradient;
                ctx.fill();

                ctx.restore();
            }

            Timer {
                interval: 16
                running: true
                repeat: true
                onTriggered: {
                    radarCanvas.rotationAngle += 1;
                    if (radarCanvas.rotationAngle >= 360)
                        radarCanvas.rotationAngle = 0;
                    radarCanvas.requestPaint();
                }
            }
        }
    }
}
