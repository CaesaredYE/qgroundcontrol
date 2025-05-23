import QtQuick 2.15
import QtQuick.Controls 2.15
import Radar.Settings 1.0

Item {
    Column {
        anchors.margins: 20
        spacing: 20

        TextField {
            placeholderText: "雷达 IP"
            text: RadarSettings.radarIP.value
            onTextChanged: RadarSettings.radarIP.value = text
        }

        TextField {
            placeholderText: "雷达端口"
            text: RadarSettings.radarPort.value.toString()
            inputMethodHints: Qt.ImhDigitsOnly
            onTextChanged: RadarSettings.radarPort.value = parseInt(text)
        }

        TextField {
            placeholderText: "本地监听端口"
            text: RadarSettings.radarLocalPort.value.toString()
            inputMethodHints: Qt.ImhDigitsOnly
            onTextChanged: RadarSettings.radarLocalPort.value = parseInt(text)
        }

        TextField {
            placeholderText: "雷达纬度"
            text: RadarSettings.radarLatitude.value.toString()
            onTextChanged: RadarSettings.radarLatitude.value = parseFloat(text)
        }

        TextField {
            placeholderText: "雷达经度"
            text: RadarSettings.radarLongitude.value.toString()
            onTextChanged: RadarSettings.radarLongitude.value = parseFloat(text)
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
            onClicked:  QGroundControl.corePlugin.connectRadar()
        }
        QGCButton {
            text:       qsTr("取消连接")
            onClicked:  {}
        }
    }
}
