/****************************************************************************
 *
 * (c) 2009-2019 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 *   @brief Custom QGCCorePlugin Implementation
 *   @author Gus Grubba <gus@auterion.com>
 */

#include <QQmlEngine>

#include "CustomPlugin.h"
#include "QmlComponentInfo.h"
#include "QGCApplication.h"

CustomPlugin::CustomPlugin(QGCApplication *app, QGCToolbox *toolbox)
    : QGCCorePlugin(app, toolbox)
{
}

void CustomPlugin::setToolbox(QGCToolbox* toolbox)
{
    QGCCorePlugin::setToolbox(toolbox);

    _radarSettings = new RadarSettings(this);
    _radarController = new RadarController(_radarSettings, this);

    qmlRegisterUncreatableType<RadarController>("QGroundControl", 1, 0, "RadarController", "Reference only");
}

QVariantList& CustomPlugin::settingsPages()
{
    if(_customSettingsList.isEmpty()) {
        _addSettingsEntry(tr("General"),     "qrc:/qml/GeneralSettings.qml",     "qrc:/res/gear-white.svg");
        _addSettingsEntry(tr("Comm Links"),  "qrc:/qml/LinkSettings.qml",        "qrc:/res/waves.svg");
        _addSettingsEntry(tr("Offline Maps"),"qrc:/qml/OfflineMap.qml",          "qrc:/res/waves.svg");
        _addSettingsEntry(tr("MAVLink"),     "qrc:/qml/MavlinkSettings.qml",     "qrc:/res/waves.svg");
        _addSettingsEntry(tr("Console"),     "qrc:/qml/QGroundControl/Controls/AppMessages.qml");
        _addSettingsEntry(tr("Radar"),       "qrc:/qml/RadarSettings.qml");
    }
    return _customSettingsList;
}

void CustomPlugin::_addSettingsEntry(const QString& title, const char* qmlFile, const char* iconFile)
{
    Q_CHECK_PTR(qmlFile);
    _customSettingsList.append(QVariant::fromValue(
        new QmlComponentInfo(title,
                             QUrl::fromUserInput(qmlFile),
                             iconFile == nullptr ? QUrl() : QUrl::fromUserInput(iconFile),
                             this)));
}

void CustomPlugin::sendTargetPosition(const QString& lat, const QString& lon, const QString& alt)
{
    Vehicle* vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    vehicle->sendMavCommandInt(MAV_COMP_ID_UDP_BRIDGE,
                               MAV_CMD_USER_1,
                               MAV_FRAME_GLOBAL,
                               false,
                               NAN,
                               NAN,
                               NAN,
                               NAN,
                               lat.toDouble(),
                               lon.toDouble(),
                               alt.toFloat());
}

void CustomPlugin::emergencyStop()
{
    Vehicle* vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    vehicle->sendMavCommand(MAV_COMP_ID_UDP_BRIDGE,
                            MAV_CMD_USER_2,
                            false,
                            NAN,
                            NAN,
                            NAN,
                            NAN,
                            NAN,
                            NAN,
                            NAN);
}

void CustomPlugin::sendUdpCommand(const QString& ip, int port, const QString& message) {
    if (!_udpSocket) {
        _udpSocket = new QUdpSocket(this);
        if (!_udpSocket->bind(QHostAddress::AnyIPv4, 6001, QAbstractSocket::ReuseAddressHint | QUdpSocket::ShareAddress)) {
            qWarning() << "UDP bind failed on port 6001:" << _udpSocket->errorString();
            _udpSocket->deleteLater();
            _udpSocket = nullptr;
            return;
        }

        if (!_udpSocket->isValid()) {
            qWarning() << "UDP socket is not valid.";
            _udpSocket->deleteLater();
            _udpSocket = nullptr;
            return;
        }

        connect(_udpSocket, &QUdpSocket::readyRead, this, &CustomPlugin::onDataReceived);
    }

    QByteArray datagram = message.toUtf8();
    qint64 bytesWritten = _udpSocket->writeDatagram(datagram, QHostAddress(ip), port);
    qDebug() << "Trying to send UDP to" << ip << ":" << port;

    if (bytesWritten == -1) {
        qWarning() << "Failed to send datagram:" << _udpSocket->errorString();
    } else {
        qDebug() << "Sent" << bytesWritten << "bytes to" << ip << ":" << port;
    }
}

void CustomPlugin::onDataReceived() {
    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(_udpSocket->pendingDatagramSize());
        _udpSocket->readDatagram(buffer.data(), buffer.size());

        QJsonObject obj = QJsonDocument::fromJson(buffer).object();
        QString cmd = obj["cmd"].toString();
        QString output = obj["output"].toString();
        emit cmdOutput(cmd, output);
    }
}
