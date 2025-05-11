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


    QTimer* pollTimer = new QTimer(this);
    connect(pollTimer, &QTimer::timeout, this, [=]() {
        qint64 sent = _udpSocket->writeDatagram(byteArray, _remoteHost, _remotePort);
        if (sent == -1) {
            qWarning() << "[UDP] Send failed:" << _udpSocket->errorString();
        }
    });
    pollTimer->start(1000); 
}

void RadarReceiver::processPendingDatagrams() {
    // QList<QGeoCoordinate> targets;
    qDebug() << "Datagram received!";

    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        qint64 bytesRead = _udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);


        if (bytesRead == -1) {
            qDebug() << "读取失败：" << _udpSocket->errorString();
        } else {
            qDebug() << "收到数据：" << datagram.toHex();
        }

        if (sender != _remoteHost || senderPort != _remotePort) continue;
        
        // QDataStream stream(datagram);
        // stream.setByteOrder(QDataStream::LittleEndian);
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
