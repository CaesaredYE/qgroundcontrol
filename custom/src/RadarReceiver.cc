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

    const uint8_t data[10] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
    QByteArray byteArray(reinterpret_cast<const char*>(data), sizeof(data));

    _udpSocket->writeDatagram(byteArray, _remoteHost, _remotePort);

    QTimer* pollTimer = new QTimer(this);
    connect(pollTimer, &QTimer::timeout, this, [=]() {
        _udpSocket->writeDatagram(byteArray, _remoteHost, _remotePort);
    });
    pollTimer->start(1000 * 60);
}

void RadarReceiver::readData() {
    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_udpSocket->pendingDatagramSize());
        qint64 bytesRead = _udpSocket->readDatagram(datagram.data(), datagram.size());

        if (bytesRead == -1) {
            qDebug() << "读取失败：" << _udpSocket->errorString();
        }

        // 航迹报文
        if (datagram.size() >= 56 && datagram.constData()[4] == 0x01) {
            qDebug() << "收到航迹报文：" << datagram.toHex();

            uint16_t trackCount;
            memcpy(&trackCount, datagram.constData() + 8, sizeof(uint16_t));

            for (int i = 0; i < trackCount; ++i) {

                int offset =  i * 80;

                TrackInfo info {};
                memcpy(&info.batch, datagram.constData() + 12 + offset, sizeof(uint32_t));
                memcpy(&info.lat, datagram.constData() + 24 + offset, sizeof(float));
                memcpy(&info.lon, datagram.constData() + 28 + offset, sizeof(float));
                memcpy(&info.alt, datagram.constData() + 52 + offset, sizeof(float));
                memcpy(&info.existFlag, datagram.constData() + 36 + offset, sizeof(uint16_t));

                qDebug() << "批号" << info.batch << "纬度" << info.lat  << "经度" << info.lon << "高度" << info.alt << "存在标识" << info.existFlag;

                updateTrack(info);
            }
        }
    }
}

void RadarReceiver::updateTrack(const TrackInfo& info) {
    for (int i = 0; i < _tracks.size(); ++i) {
        if (_tracks[i].batch == info.batch) {
            _tracks[i] = info;
            emit trackListChanged();
            return;
        }
    }
    _tracks.append(info);
    emit trackListChanged();
}

QVariantList RadarReceiver::trackList() const {
    QVariantList list;
    for (const TrackInfo& t : _tracks) {
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
