#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUdpSocket>
#include <QGeoCoordinate>
#include "RadarSettings.h" 

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
    void sendTrackToVehicle             ();
    void convertEcefToLla              (double x, double y, double z, double& lat, double& lon, double& alt);

private:
    QUdpSocket* _udpSocket          = nullptr;
    RadarSettings* _radarSettings   = nullptr; 

    QHostAddress _remoteHost;
    quint16 _remotePort;
    quint16 _localPort;

    QMap<quint32, QJsonObject> _tracks;
    quint32 _targetBatch            = 0;

    bool _isConnected               = false;
    bool _isScanning                = false;
    bool _isPlane                   = false;
};
