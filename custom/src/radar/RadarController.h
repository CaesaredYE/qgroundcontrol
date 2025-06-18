#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QGeoCoordinate>
#include "RadarSettings.h" 

struct TrackInfo {
    quint32 batch;      // 批号
    quint16 existFlag;  // 存在标识

    float compass;      // 方位
    float distance;     // 距离
    float course;       // 航向
    float speed;        // 航速
    
    float lat;          // 纬度
    float lon;          // 经度
    float alt;          // 高度
};

class RadarController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList trackList READ trackList NOTIFY trackListChanged)
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(bool isScanning READ isScanning NOTIFY isScanningChanged)

public:
    explicit RadarController(RadarSettings* radarSettings, QObject* parent = nullptr);

    QVariantList trackList              () const;
    bool isConnected                    (void) const { return _isConnected; }
    bool isScanning                     (void) const { return _isScanning; }

    Q_INVOKABLE void connectRadar       ();
    Q_INVOKABLE void disconnectRadar    ();
    Q_INVOKABLE void startScan          ();
    Q_INVOKABLE void stopScan           ();
    Q_INVOKABLE void setTargetBatch     (quint32 batch);
    Q_INVOKABLE void confirmTarget      ();

signals:
    void trackListChanged               ();
    void isConnectedChanged             ();
    void isScanningChanged              ();

private slots:
    void onDataReceived                 ();

private:
    void startHeartbeat                 ();
    void stopHeartbeat                  ();
    void sendData                       (const uint8_t* data, int length);
    void sendTrackToVehicle             ();

private:
    QUdpSocket* _udpSocket          = nullptr;
    QTimer* _heartbeatTimer         = nullptr;
    RadarSettings* _radarSettings   = nullptr; 

    QHostAddress _remoteHost;
    quint16 _remotePort;
    quint16 _localPort;

    QMap<quint32, TrackInfo> _tracks;
    quint32 _targetBatch            = 0;

    bool _isConnected               = false;
    bool _isScanning                = false;
    bool _isPlane                   = false;
};
