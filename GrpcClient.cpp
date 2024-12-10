#include "GrpcClient.h"

#include <QDebug>

GrpcClient::GrpcClient(QObject *parent) : QObject { parent }, _pingTimer { new QTimer(this) }
{
    _pingTimer->setInterval(5000);
    connect(_pingTimer, &QTimer::timeout, this, &GrpcClient::pingServer);
}

void GrpcClient::connectToServer(const QString &serverIp, const quint16 serverPort)
{
    if (_isGrpcConnected) {
        qInfo() << "Already connected to gRPC server.";
        emit stopBroadcast();
        return;
    }

    QString grpcAddress = serverIp + ":" + QString::number(serverPort);
    qInfo() << "Connecting to gRPC server at" << grpcAddress;

    _channel = grpc::CreateChannel(grpcAddress.toStdString(), grpc::InsecureChannelCredentials());
    _stub = MaintainingApi::MaintainingApi::NewStub(_channel);

    _isGrpcConnected = true;

    _pingFailureCount = 0;
    _pingTimer->start();
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

        emit pingResponseReceived(pingResponse.response().c_str());
    } else {
        qWarning() << "gRPC call failed:" << status.error_message().c_str();

        _pingFailureCount++;

        if (_pingFailureCount >= 3) {
            qWarning() << "3 consecutive ping failures. Stopping ping cycle.";
            _pingTimer->stop();
            emit resumeBroadcast();
            _isGrpcConnected = false;
        }
    }
}
