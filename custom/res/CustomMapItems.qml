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
import RadarReceiver 1.0

Item {
    id: radar
    property var    map
    property bool   largeMapView
    property var    radarCenter: QtPositioning.coordinate(30.276150, 114.070351)
    property var    selectedTrack: null

    Component.onCompleted: {
        if (map) {
            map.center = radarCenter;
        }
    }

    // 目标列表
    MapItemView {
        id: mapItemView
        parent: map
        model: RadarReceiver.trackList
        delegate: MapQuickItem {
            parent: map
            visible: modelData.existFlag === 1
            anchorPoint.x: 5
            anchorPoint.y: 5
            coordinate: QtPositioning.coordinate(modelData.lat, modelData.lon)
            z: QGroundControl.zOrderMapItems
            sourceItem: Rectangle {
                width: 20
                height: 20
                radius: 20
                color: "red"
                border.color: "black"
                border.width: 1
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (radar.selectedTrack && (radar.selectedTrack.batch === modelData.batch)){
                            radar.selectedTrack = null;
                        } else {
                            radar.selectedTrack = modelData;
                        }
                    }
                }
            }
        }
    }

    // 选为目标按钮
    MapQuickItem {
        parent: map
        visible: radar.selectedTrack !== null
        coordinate: radar.selectedTrack !== null
                    ? QtPositioning.coordinate(radar.selectedTrack.lat, radar.selectedTrack.lon)
                    : QtPositioning.coordinate(0, 0)
        anchorPoint.x: 0
        anchorPoint.y: 40
        z: QGroundControl.zOrderWidgets

        sourceItem: QGCButton {
            text: "选为目标"
            onClicked: {
                RadarReceiver.setTargetBatch(radar.selectedTrack.batch)
                radar.selectedTrack = null;
            }
        }
    }

    // 雷达中心点
    MapQuickItem {
        parent: map
        coordinate: radarCenter
        anchorPoint.x: radarScan.width / 2
        anchorPoint.y: radarScan.height / 2
        sourceItem: radarScan
        z: QGroundControl.zOrderMapItems
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
                ctx.strokeStyle = "rgba(255, 0, 0, 0.7)";
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
                gradient.addColorStop(0, "rgba(0, 255, 0, 0.5)");
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
