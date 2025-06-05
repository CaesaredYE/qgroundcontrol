#include "RadarController.h"
#include <QDataStream>
#include <QDebug>
#include <QtMath>
#include <QTimer>

RadarController::RadarController(RadarSettings* radarSettings,  QObject* parent)
    : QObject(parent)
    , _radarSettings(radarSettings)
{
    Q_ASSERT(_radarSettings);

    _remoteHost = QHostAddress(_radarSettings->radarIP()->rawValue().toString());
    _remotePort = _radarSettings->radarPort()->rawValue().toUInt();
    _localPort  = _radarSettings->radarLocalPort()->rawValue().toUInt();
}

QVariantList RadarController::trackList() const {
    QVariantList list;
    for (const auto& t : _tracks) {
        QVariantMap map;
        map["batch"] = t.batch;
        map["existFlag"] = t.existFlag;

        map["compass"] = QString::number(t.compass);
        map["distance"] = QString::number(t.distance);
        map["course"] = QString::number(t.course);
        map["speed"] = QString::number(t.speed);

        map["lat"] = QString::number(t.lat);
        map["lon"] = QString::number(t.lon);
        map["alt"] = QString::number(t.alt);

        list.append(map);
    }
    return list;
}

void RadarController::connectRadar() {
    if (_udpSocket) {
        return;
    }

    _udpSocket = new QUdpSocket(this);
    if (!_udpSocket->bind(QHostAddress::AnyIPv4, _localPort)) {
        _udpSocket->deleteLater();
        _udpSocket = nullptr;
        return;
    }

    connect(_udpSocket, &QUdpSocket::readyRead, this, &RadarController::onDataReceived);

    _isConnected = true;
    emit isConnectedChanged();
}

void RadarController::disconnectRadar() {
    if (_udpSocket) {
        _udpSocket->close();
        _udpSocket->deleteLater();
        _udpSocket = nullptr;
    }

    _isConnected = false;
    emit isConnectedChanged();

    stopHeartbeat();
}

void RadarController::startScan() {
    const uint8_t data[] = {
        0xaa, 0xaa, 0xaa, 0xaa,
        0x0b, 0x00, 0x00, 0x00,
        0x15, 0x01, 0x00, 0xff,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0xff, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    sendData(data, sizeof(data));

    _isScanning = true;
    emit isScanningChanged();

    startHeartbeat();
}

void RadarController::stopScan()
{
    const uint8_t data[] = {
        0xaa, 0xaa, 0xaa, 0xaa,
        0x0b, 0x00, 0x00, 0x00,
        0x15, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    sendData(data, sizeof(data));

    _isScanning = false;
    emit isScanningChanged();

    stopHeartbeat();
}

void RadarController::setTargetBatch(quint32 batch) {
    qDebug() << "Set target batch:" << batch;
    _targetBatch = batch;
    sendTrackToVehicle();
}

void RadarController::onDataReceived() {
    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_udpSocket->pendingDatagramSize());
        qint64 bytesRead = _udpSocket->readDatagram(datagram.data(), datagram.size());

        if (bytesRead == -1) {
            qWarning() << "receive fail:" << _udpSocket->errorString();
        }

        // 航迹报文
        if (datagram.size() >= 56 && datagram.constData()[4] == 0x01) {
            quint16 trackCount;
            memcpy(&trackCount, datagram.constData() + 8, sizeof(quint16));

            for (int i = 0; i < trackCount; ++i) {
                int offset =  i * 80;

                TrackInfo info {};
                memcpy(&info.batch, datagram.constData() + 12 + offset, sizeof(quint32));
                memcpy(&info.existFlag, datagram.constData() + 36 + offset, sizeof(quint16));

                memcpy(&info.compass, datagram.constData() + 16 + offset, sizeof(float));
                memcpy(&info.distance, datagram.constData() + 20 + offset, sizeof(float));
                memcpy(&info.course, datagram.constData() + 44 + offset, sizeof(float));
                memcpy(&info.speed, datagram.constData() + 48 + offset, sizeof(float));

                memcpy(&info.lat, datagram.constData() + 24 + offset, sizeof(float));
                memcpy(&info.lon, datagram.constData() + 28 + offset, sizeof(float));
                memcpy(&info.alt, datagram.constData() + 52 + offset, sizeof(float));

                _tracks[info.batch] = info;

                qDebug() << "收到雷达消息; 批号" << info.batch << "纬度" << info.lat  << "经度" << info.lon << "高度" << info.alt << "存在标识" << info.existFlag;

                if (_targetBatch && info.batch == _targetBatch) {
                    sendTrackToVehicle();
                }
            }

            emit trackListChanged();
        }
    }
}

void RadarController::startHeartbeat() {
    if (_heartbeatTimer) {
        return;
    }

    const uint8_t data[] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
    sendData(data, sizeof(data));

    _heartbeatTimer = new QTimer(this);
    connect(_heartbeatTimer, &QTimer::timeout, this, [=]() {
        sendData(data, sizeof(data));
    });
    _heartbeatTimer->start(60 * 1000);
}

void RadarController::stopHeartbeat() {
    if (_heartbeatTimer) {
        _heartbeatTimer->stop();
        _heartbeatTimer->deleteLater();
        _heartbeatTimer = nullptr;
    }
}

void RadarController::sendData(const uint8_t* data, int length) {
    if (!_udpSocket) {
        return;
    }

    QByteArray byteArray(reinterpret_cast<const char*>(data), length);
    _udpSocket->writeDatagram(byteArray, _remoteHost, _remotePort);
}

void RadarController::sendTrackToVehicle() {
    if (_targetBatch == 0 || !_tracks.contains(_targetBatch)) {
        return;
    }

    const TrackInfo& target = _tracks[_targetBatch];

    Vehicle* vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    if (!vehicle) {
        return;
    }

    qDebug() << "Send track to vehicle: batch" << target.batch << "lat" << target.lat << "lon" << target.lon << "alt" << target.alt;

    vehicle->sendMavCommand(MAV_COMP_ID_UDP_BRIDGE,
                            MAV_CMD_USER_1,
                            false,
                            NAN,
                            NAN,
                            target.batch,
                            target.existFlag,
                            target.lat,
                            target.lon,
                            target.alt);
}
