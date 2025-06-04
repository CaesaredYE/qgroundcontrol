import QtQuick 2.15
import QtQuick.Controls 2.15
import QGroundControl.Controls 1.0
import QGroundControl.FactSystem 1.0
import QGroundControl.FactControls  1.0
import QGroundControl.SettingsManager   1.0

Item {
    id: root

    property var radarSettings: QGroundControl.settingsManager.radarSettings

    Column {
        spacing: 16
        anchors.margins: 20

        FactTextField {
            label: qsTr("Radar IP Address")
            fact: radarSettings.radarIP
        }

        FactTextField {
            label: qsTr("Radar Port")
            fact: radarSettings.radarPort
        }

        FactTextField {
            label: qsTr("Radar Local Port")
            fact: radarSettings.radarLocalPort
        }

        FactTextField {
            label: qsTr("Radar Latitude")
            fact: radarSettings.radarLatitude
        }

        FactTextField {
            label: qsTr("Radar Longitude")
            fact: radarSettings.radarLongitude
        }
    }
}
