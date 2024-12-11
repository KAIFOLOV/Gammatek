#include "GrpcClient.h"

#include <QDebug>
#include <QDateTime>

GrpcClient::GrpcClient(QObject *parent) : QObject { parent }, _pingTimer { new QTimer(this) }
{
    _pingTimer->setInterval(5000);
    connect(_pingTimer, &QTimer::timeout, this, &GrpcClient::pingServer);
}

void GrpcClient::connectToServer()
{
    if (_isGrpcConnected) {
        qInfo() << "Already connected to gRPC server.";
        return;
    }

    QString grpcAddress = _ip + ":" + QString::number(_port);
    qInfo() << "Connecting to gRPC server at" << grpcAddress;

    _channel = grpc::CreateChannel(grpcAddress.toStdString(), grpc::InsecureChannelCredentials());
    _stub = MaintainingApi::MaintainingApi::NewStub(_channel);

    _isGrpcConnected = true;

    _pingFailureCount = 0;
    _pingTimer->start();

    emit connectionStateChanged();
}

void GrpcClient::disconnectFromServer()
{
    if (!_isGrpcConnected) {
        qInfo() << "Already disconnected from gRPC server.";
        return;
    }

    qInfo() << "Disconnecting from gRPC server.";

    _pingTimer->stop();
    _channel.reset();
    _stub.reset();
    _isGrpcConnected = false;

    emit connectionStateChanged();
}

void GrpcClient::pingServer()
{
    if (!_isGrpcConnected) {
        qWarning() << "gRPC client is not connected!";
        return;
    }

    PingRequest pingRequest;
    pingRequest.set_clientip("1");

    PingResponse pingResponse;
    grpc::ClientContext context;

    grpc::Status status = _stub->Ping(&context, pingRequest, &pingResponse);

    if (status.ok()) {
        qInfo() << "Ping response:" << pingResponse.response().c_str();

        _pingFailureCount = 0;
        _lastPingTime = QDateTime::currentDateTime().toString("HH:mm:ss");

        emit lastPingTimeChanged();
    } else {
        qWarning() << "gRPC call failed:" << status.error_message().c_str();

        _pingFailureCount++;

        if (_pingFailureCount >= 3) {
            qWarning() << "3 consecutive ping failures. Stopping ping cycle.";
            disconnectFromServer();
        }
    }
}

QString GrpcClient::ip() const
{
    return _ip;
}

quint16 GrpcClient::port() const
{
    return _port;
}

QString GrpcClient::lastPingTime() const
{
    return _lastPingTime;
}

bool GrpcClient::isGrpcConnected() const
{
    return _isGrpcConnected;
}

void GrpcClient::setIp(const QString &newIp)
{
    _ip = newIp;
}

void GrpcClient::setPort(quint16 newPort)
{
    _port = newPort;
}
