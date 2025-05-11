#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QGeoCoordinate>

class RadarReceiver : public QObject {
    Q_OBJECT
public:
    explicit RadarReceiver(QObject* parent = nullptr);
    void startConnection(const QString& remoteIp, quint16 remotePort, quint16 localPort = 8000);

signals:
    void radarDataReceived(float angle, float distance, float intensity);
    void radarTargetsUpdated(const QList<QGeoCoordinate>& targets);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket* _udpSocket;
    QHostAddress _remoteHost;
    quint16 _remotePort;
};