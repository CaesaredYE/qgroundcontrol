#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QGeoCoordinate>

struct TrackInfo {
    uint32_t batch; // 批号
    float lat; // 纬度
    float lon; // 经度
    float alt; // 高度
    uint16_t existFlag; // 存在标识
};

class RadarReceiver : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList trackList READ trackList NOTIFY trackListChanged)

public:
    explicit RadarReceiver(QObject* parent = nullptr);
    QVariantList trackList() const;

signals:
    void trackListChanged();

public slots:
    void start();

private slots:
    void readData();

private:
    QUdpSocket* _udpSocket = nullptr;
    QHostAddress _remoteHost = QHostAddress("localhost");
    quint16 _remotePort = 12580;
    quint16 _localPort = 8000; 

    QList<TrackInfo> _tracks;
    
    void updateTrack(const TrackInfo& info);
};
