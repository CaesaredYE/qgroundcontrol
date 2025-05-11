#include "RadarReceiver.h"
#include <QDataStream>
#include <QDebug>
#include <QtMath>
#include <QTimer>

RadarReceiver::RadarReceiver(QObject* parent)
    : QObject(parent), _udpSocket(new QUdpSocket(this)) {}

void RadarReceiver::startConnection(const QString& remoteIp, quint16 remotePort, quint16 localPort) {
    _remoteHost = QHostAddress(remoteIp);
    _remotePort = remotePort;

    if (!_udpSocket->bind(QHostAddress::AnyIPv4, localPort)) {
        qWarning() << "Failed to bind UDP port for radar";
        return;
    }

    connect(_udpSocket, &QUdpSocket::readyRead, this, &RadarReceiver::processPendingDatagrams);

    const uint8_t data[10] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
    QByteArray byteArray(reinterpret_cast<const char*>(data), sizeof(data));

    _udpSocket->writeDatagram(byteArray, _remoteHost, _remotePort);
 
    QTimer* pollTimer = new QTimer(this);
    connect(pollTimer, &QTimer::timeout, this, [=]() {
        _udpSocket->writeDatagram(byteArray, _remoteHost, _remotePort);
    });
    pollTimer->start(1000 * 60);
}

struct TrackInfo {
    uint32_t batch; // 批号
    float lat; // 纬度
    float lon; // 经度
    float alt; // 高度
    uint16_t existFlag; // 存在标识
};

void RadarReceiver::processPendingDatagrams() {
    // QList<QGeoCoordinate> targets;

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

            TrackInfo info {};
            info.batch = datagram.constData()[7];
            memcpy(&info.lat, datagram.constData() + 24, sizeof(float));
            memcpy(&info.lon, datagram.constData() + 28, sizeof(float));
            info.existFlag = datagram.constData()[36];
            memcpy(&info.alt, datagram.constData() + 52, sizeof(float));

            qDebug() << "批号" << info.batch;
            qDebug() << "纬度" << info.lat;
            qDebug() << "经度" << info.lon;
            qDebug() << "高度" << info.alt;
            qDebug() << "存在标识" << info.existFlag;
        }

        // float angle, distance, intensity;
        // stream >> angle >> distance >> intensity;

        // emit radarDataReceived(angle, distance, intensity);

        // // Optional: convert to GPS (dummy location for example)
        // QGeoCoordinate origin(39.9042, 116.4074); // e.g., Beijing
        // double angleRad = qDegreesToRadians(angle);
        // double earthRadius = 6378137.0;
        // double deltaLat = (distance * cos(angleRad)) / earthRadius;
        // double deltaLon = (distance * sin(angleRad)) / (earthRadius * cos(qDegreesToRadians(origin.latitude())));
        // QGeoCoordinate targetCoord(
        //     origin.latitude() + qRadiansToDegrees(deltaLat),
        //     origin.longitude() + qRadiansToDegrees(deltaLon)
        // );
        // targets.append(targetCoord);
    }

    // if (!targets.isEmpty()) {
    //     emit radarTargetsUpdated(targets);
    // }
}
