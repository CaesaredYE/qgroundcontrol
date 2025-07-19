#include "RadarController.h"
#include "QGCApplication.h"

#include <QDataStream>
#include <QDebug>
#include <QtMath>
#include <QTimer>

RadarController::RadarController(RadarSettings* radarSettings,  QObject* parent)
    : QObject(parent)
    , _radarSettings(radarSettings)
{
    Q_ASSERT(_radarSettings);

    bool autoConnect  = _radarSettings->autoConnect()->rawValue().toBool();
    if (autoConnect) {
        connectRadar();
    }
}

QVariantList RadarController::trackList() const {
    QVariantList list;
    for (const QJsonObject& t : _tracks) {
        list.append(t);
    }
    return list;
}

void RadarController::connectRadar() {
    if (_udpSocket) {
        return;
    }

    _remoteHost = QHostAddress(_radarSettings->ip()->rawValue().toString());
    _remotePort = _radarSettings->port()->rawValue().toUInt();
    _localPort  = _radarSettings->localPort()->rawValue().toUInt();

    _udpSocket = new QUdpSocket(this);
    if (!_udpSocket->bind(QHostAddress::AnyIPv4, _localPort, QAbstractSocket::ReuseAddressHint | QUdpSocket::ShareAddress)) {
        qWarning() << "Failed to bind UDP socket:" << _udpSocket->errorString() << _localPort;
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
        if(_isScanning){
            stopScan();
        }

        _udpSocket->close();
        _udpSocket->deleteLater();
        _udpSocket = nullptr;
    }

    _isConnected = false;
    emit isConnectedChanged();
}

void RadarController::startScan() {
    _isScanning = true;
    emit isScanningChanged();
}

void RadarController::stopScan()
{
    _tracks.clear();
    _isScanning = false;
    emit isScanningChanged();
}

void RadarController::setTargetBatch(quint32 batch) {
    qDebug() << "Set target batch:" << batch;
    _targetBatch = batch;
    sendTrackToVehicle();
}

void RadarController::confirmTarget() {
    _isPlane = !_isPlane;
    sendTrackToVehicle();
    qDebug() << "Set target is plane:" << _isPlane;
}

void RadarController::sendTrackToVehicle() {
    if (_targetBatch == 0 || !_tracks.contains(_targetBatch)) {
        return;
    }

    const QJsonObject& target = _tracks[_targetBatch];

    Vehicle* vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    if (!vehicle) {
        return;
    }

    qDebug() << "Send track to vehicle: batch" << target["batch"] << "lat" << target["lat"] << "lon" << target["lon"] << "alt" << target["alt"];

    vehicle->sendMavCommand(MAV_COMP_ID_UDP_BRIDGE,
                            MAV_CMD_USER_1,
                            false,
                            NAN,
                            _isPlane == true ? 1 : 0,
                            target["batch"].toDouble(),
                            target["guid_status"].toDouble(),
                            target["lat"].toDouble(),
                            target["lon"].toDouble(), 
                            target["alt"].toDouble());
}

void RadarController::onDataReceived() {
    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_udpSocket->pendingDatagramSize());
        qint64 bytesRead = _udpSocket->readDatagram(datagram.data(), datagram.size());

        if (bytesRead == -1) {
            qWarning() << "receive fail:" << _udpSocket->errorString();
            continue;
        }

        if(!_isScanning){
            continue;
        }

        if (datagram.size() < 32) {
            qWarning() << "datagram too small:" << datagram.size();
            continue;
        }

        quint16 messageId;
        memcpy(&messageId, datagram.constData() + 0, sizeof(quint16));
        qDebug() << "Message ID:" << QString::number(messageId, 16);

        if (messageId != 0x7104) {
            continue;
        }
        
        quint16 bodyInfo;
        memcpy(&bodyInfo, datagram.constData() + 28, sizeof(quint16)); 
        quint16 bodyType = (bodyInfo >> 12) & 0x0F;
        quint16 bodyLength = bodyInfo & 0x0FFF;
        qDebug() << "Body Type:" << bodyType ;
        qDebug() << "Body Length:" << bodyLength;

        if (bodyType != 0) {
            qDebug() << "Skip non-JSON format, body type:" << bodyType;
            continue;
        }

        if (bodyLength == 0) {
            qDebug() << "Body length is 0";
            continue;
        }

        quint16 bodyCount;
        memcpy(&bodyCount, datagram.constData() + 30, sizeof(quint16)); 
        qDebug() << "Body Count:" << bodyCount;
        
        int bodyStart = 32;
        for (int i = 0; i < bodyCount; ++i) {
            if (bodyStart + bodyLength > datagram.size()) {
                qWarning() << "Body data exceeds datagram size";
                continue;
            }
            
            QByteArray jsonData = datagram.mid(bodyStart, bodyLength);
            qDebug() << "JSON body" << i << ":" << jsonData;
            
            QJsonParseError parseError;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            
            if (parseError.error != QJsonParseError::NoError) {
                qWarning() << "JSON parse error:" << parseError.errorString();
                bodyStart += bodyLength;
                continue;
            }
            
            if (jsonDoc.isObject()) {
                QJsonObject jsonObj = jsonDoc.object();

                quint32 batch = jsonObj["tar_id"].toVariant().toUInt();
                jsonObj["batch"] = QJsonValue(static_cast<qint64>(batch));
                
                double x = jsonObj["ecef_x"].toDouble();
                double y = jsonObj["ecef_y"].toDouble();
                double z = jsonObj["ecef_z"].toDouble();
                
                double lat, lon, alt;
                convertEcefToLla(x, y, z, lat, lon, alt);
                
                jsonObj["lat"] = lat;
                jsonObj["lon"] = lon;
                jsonObj["alt"] = alt;
                
                qDebug() << "ECEF to LLA conversion:";
                qDebug() << "  ECEF (x,y,z):" << x << y << z;
                qDebug() << "  LLA (lat,lon,alt):" << lat << lon << alt;

                _tracks[batch] = jsonObj;

                if (_targetBatch && batch == _targetBatch) {
                    sendTrackToVehicle();
                }
                
                emit trackListChanged();
            }
            
            bodyStart += bodyLength;
        }

    }
}

void RadarController::convertEcefToLla(double x, double y, double z, double& lat, double& lon, double& alt) {
    // CGCS2000椭球体参数 (与WGS84相同)
    const double a = 6378137.0;        // 长半轴 (米)
    const double f = 1.0 / 298.257222101;  // 扁率 (CGCS2000)
    const double e2 = 2.0 * f - f * f; // 第一偏心率平方
    
    // 计算经度
    lon = qAtan2(y, x);
    
    // 计算纬度 (迭代方法)
    double p = qSqrt(x * x + y * y);
    double lat_prev = qAtan2(z, p * (1.0 - e2));
    
    for (int i = 0; i < 10; ++i) {
        double sin_lat = qSin(lat_prev);
        double N = a / qSqrt(1.0 - e2 * sin_lat * sin_lat);
        double lat_new = qAtan2(z + e2 * N * sin_lat, p);
        
        if (qAbs(lat_new - lat_prev) < 1e-12) {
            lat = lat_new;
            break;
        }
        lat_prev = lat_new;
    }
    lat = lat_prev;
    
    // 计算高度
    double sin_lat = qSin(lat);
    double N = a / qSqrt(1.0 - e2 * sin_lat * sin_lat);
    alt = p / qCos(lat) - N;
    
    // 转换为度
    lat = lat * 180.0 / M_PI;
    lon = lon * 180.0 / M_PI;
}

