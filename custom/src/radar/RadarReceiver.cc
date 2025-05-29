#include "RadarReceiver.h"
#include <QDataStream>
#include <QDebug>
#include <QtMath>
#include <QTimer>

RadarReceiver::RadarReceiver(QObject* parent)
    : QObject(parent) 
{
}

void RadarReceiver::start() {
    _udpSocket = new QUdpSocket(this);

    if (!_udpSocket->bind(QHostAddress::AnyIPv4, _localPort)) {
        qWarning() << "Failed to bind local UDP port for radar";
        return;
    }

    connect(_udpSocket, &QUdpSocket::readyRead, this, &RadarReceiver::readData);

    const uint8_t data[] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};

    QTimer* pollTimer = new QTimer(this);
    connect(pollTimer, &QTimer::timeout, this, [=]() {
        writeData(data, sizeof(data));
    });
    writeData(data, sizeof(data));

    pollTimer->start(1000 * 60);
}

void RadarReceiver::writeData(const uint8_t* data, int length) {
    QByteArray byteArray(reinterpret_cast<const char*>(data), length);
    qDebug() << "Send radar command:" << byteArray.toHex();
    _udpSocket->writeDatagram(byteArray, _remoteHost, _remotePort);
}

void RadarReceiver::readData() {
    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_udpSocket->pendingDatagramSize());
        qint64 bytesRead = _udpSocket->readDatagram(datagram.data(), datagram.size());

        if (bytesRead == -1) {
            qWarning() << "receive fail:" << _udpSocket->errorString();
        }

        // 航迹报文
        if (datagram.size() >= 56 && datagram.constData()[4] == 0x01) {
            qDebug() << "received:" << datagram.toHex();

            quint16 trackCount;
            memcpy(&trackCount, datagram.constData() + 8, sizeof(quint16));

            for (int i = 0; i < trackCount; ++i) {
                int offset =  i * 80;

                TrackInfo info {};
                memcpy(&info.batch, datagram.constData() + 12 + offset, sizeof(quint32));
                memcpy(&info.lat, datagram.constData() + 24 + offset, sizeof(float));
                memcpy(&info.lon, datagram.constData() + 28 + offset, sizeof(float));
                memcpy(&info.alt, datagram.constData() + 52 + offset, sizeof(float));
                memcpy(&info.existFlag, datagram.constData() + 36 + offset, sizeof(quint16));

                qDebug() << "批号" << info.batch << "纬度" << info.lat  << "经度" << info.lon << "高度" << info.alt << "存在标识" << info.existFlag;

                _tracks[info.batch] = info;

                if (_targetBatch && info.batch == _targetBatch) {
                    sendTrackToVehicle();
                }
            }

            emit trackListChanged();
        }
    }
}

QVariantList RadarReceiver::trackList() const {
    QVariantList list;
    for (const auto& t : _tracks) {
        QVariantMap map;
        map["batch"] = t.batch;
        map["lat"] = t.lat;
        map["lon"] = t.lon;
        map["alt"] = t.alt;
        map["existFlag"] = t.existFlag;
        list.append(map);
    }
    return list;
}

void RadarReceiver::setTargetBatch(quint32 batch)
{
    qDebug() << "set target batch:" << batch;
    _targetBatch = batch;
    sendTrackToVehicle();
}

void RadarReceiver::sendTrackToVehicle()
{
    if (_targetBatch == 0) {
        return;
    }

    TrackInfo target = _tracks[_targetBatch];

    Vehicle* vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    if (!vehicle) {
        return;
    }

    qDebug() << "set to vehicle:" << _targetBatch << target.lat << target.lon << target.alt;

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
