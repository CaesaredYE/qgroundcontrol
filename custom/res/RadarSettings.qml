import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts  1.2

import QGroundControl   1.0
import QGroundControl.Controls  1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FactControls  1.0
import QGroundControl.Palette       1.0

Rectangle {
    id:                 root
    color:              qgcPal.window
    anchors.fill:       parent
    anchors.margins:    ScreenTools.defaultFontPixelWidth

    property var radarController:   QGroundControl.corePlugin.radarController
    property var radarSettings:     QGroundControl.corePlugin.radarSettings

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
        contentWidth:       root.width
        flickableDirection: Flickable.VerticalFlick

        Column {
            id:                 settingsColumn
            width:              root.width
            anchors.margins:    ScreenTools.defaultFontPixelWidth
            spacing:            ScreenTools.defaultFontPixelHeight / 2

            QGCLabel {
                text: qsTr("雷达IP")
            }
            FactTextField {
                fact: radarSettings.radarIP
            }

            QGCLabel {
                text: qsTr("雷达Port")
            }
            FactTextField {
                fact: radarSettings.radarPort
            }

            QGCLabel {
                text:qsTr("本机Port")
            }
            FactTextField {
                fact: radarSettings.radarLocalPort
            }

            QGCLabel {
                text: qsTr("雷达纬度")
            }
            FactTextField {
                fact: radarSettings.radarLatitude
            }

            QGCLabel {
                text: qsTr("雷达经度")
            }
            FactTextField {
                fact: radarSettings.radarLongitude
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
