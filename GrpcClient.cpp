#include "GrpcClient.h"

#include <QDebug>

GrpcClient::GrpcClient(QObject *parent) : QObject { parent }
{}

void GrpcClient::connectToServer(const QString &serverIp, quint16 serverPort)
{
    if (_isGrpcConnected) {
        qInfo() << "Already connected to gRPC server.";
        return;
    }

    // Формируем адрес для gRPC
    QString grpcAddress = serverIp + ":" + QString::number(serverPort);
    qInfo() << "Connecting to gRPC server at" << grpcAddress;

    // Создаем канал подключения с сервером
    _channel = grpc::CreateChannel(grpcAddress.toStdString(), grpc::InsecureChannelCredentials());
    _stub = MaintainingApi::MaintainingApi::NewStub(
     _channel); // Используем сгенерированный Stub внутри пространства имен MaintainingApi

    _isGrpcConnected = true;
}

void GrpcClient::pingServer()
{
    if (!_isGrpcConnected) {
        qWarning() << "gRPC client is not connected!";
        return;
    }

    PingRequest pingRequest;
    pingRequest.set_clientip("clientIp");

    PingResponse pingResponse;
    grpc::ClientContext context;

    grpc::Status status = _stub->Ping(&context, pingRequest, &pingResponse);

    if (status.ok()) {
        qInfo() << "Ping response:" << pingResponse.response().c_str();
    } else {
        qCritical() << "gRPC call failed:" << status.error_message().c_str();
    }
}
