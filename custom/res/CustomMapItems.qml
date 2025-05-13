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
    property var    targetBatch: null
    property var    _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle

    Component.onCompleted: {
        if (map) {
            map.center = radarCenter;
        }

        RadarReceiver.start()
    }

    Connections {
        target: RadarReceiver
        onTrackListChanged: {
            console.log("trackList updated:")
            for (let i = 0; i < RadarReceiver.trackList.length; ++i) {
                let item = RadarReceiver.trackList[i]
                console.log(`Track ${i}:`, JSON.stringify(item))

                if (radar.targetBatch && item.batch == radar.targetBatch && item.existFlag) {
                    QGroundControl.corePlugin.sendTargetLocation(_activeVehicle, item.lat, item.lon, item.alt);
                }
            }
        }
    }

    // 目标列表
    MapItemView {
        parent: map
        model: RadarReceiver.trackList
        delegate: MapQuickItem {
            parent: map
            visible: modelData.existFlag == 1
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
                        console.log("Clicked Track:", modelData.batch)
                        radar.selectedTrack = radar.selectedTrack ? null: modelData
                    }
                }
            }
            Component.onCompleted: {
                console.log("Track Created:", modelData.batch)
            }
        }
    }

    // 显示点击按钮
    MapQuickItem {
        parent: map
        visible: radar.selectedTrack !== null
        coordinate: radar.selectedTrack !== null
                    ? QtPositioning.coordinate(radar.selectedTrack.lat, radar.selectedTrack.lon)
                    : QtPositioning.coordinate(0, 0)
        anchorPoint.x: 0
        anchorPoint.y: 40
        z: QGroundControl.zOrderWidgets

        sourceItem: Button {
            text: "选为目标"
            onClicked: {
                console.log("Target Track batch:", radar.selectedTrack.batch)
                radar.targetBatch = radar.selectedTrack.batch
                radar.selectedTrack = null
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

    // 扫描视图（在雷达中心绘制）
    Item {
        id: radarScan
        width: 500
        height: 500

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
                ctx.rotate(rotationAngle * Math.PI / 180);

                var gradient = ctx.createRadialGradient(0, 0, 0, 0, 0, width / 2);
                gradient.addColorStop(0, "rgba(255, 0, 0, 0.4)");
                gradient.addColorStop(1, "rgba(0, 255, 0, 0)");

                ctx.beginPath();
                ctx.moveTo(0, 0);
                ctx.arc(0, 0, width / 2, 0, sweepAngle * Math.PI / 180);
                ctx.closePath();
                ctx.fillStyle = gradient;
                ctx.fill();

                ctx.restore();
            }

            Timer {
                interval: 2
                running: true
                repeat: true
                onTriggered: {
                    radarCanvas.rotationAngle += 1;
                    if (radarCanvas.rotationAngle > 360)
                        radarCanvas.rotationAngle = 0;
                    radarCanvas.requestPaint();
                }
            }
        }
    }
}
