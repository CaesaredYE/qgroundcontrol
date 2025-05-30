#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QGeoCoordinate>
#include "QGCApplication.h"
#include "Vehicle.h"

struct TrackInfo {
    quint32 batch; // 批号
    quint16 existFlag; // 存在标识

    float compass; // 方位
    float distance; // 距离
    float course; // 航向
    float speed; // 航速
    
    float lat; // 纬度
    float lon; // 经度
    float alt; // 高度
};

class RadarReceiver : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList trackList READ trackList NOTIFY trackListChanged)

public:
    explicit RadarReceiver(QObject* parent = nullptr);
    QVariantList trackList() const;

    void start();
    void writeData(const uint8_t* data, int length);

    Q_INVOKABLE void setTargetBatch(quint32 batch);

signals:
    void trackListChanged();

private slots:
    void readData();

private:
    void sendTrackToVehicle();

private:
    QUdpSocket* _udpSocket = nullptr;
    QHostAddress _remoteHost = QHostAddress("192.168.0.100");
    quint16 _remotePort = 12580;
    quint16 _localPort = 8000;

    QMap<quint32, TrackInfo> _tracks;

    quint32 _targetBatch = 0;
};
