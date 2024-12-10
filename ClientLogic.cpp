#include "ClientLogic.h"

ClientLogic::ClientLogic(QPointer<GrpcClient> grpcClient,
                         QPointer<UdpClient> udpClient,
                         QObject *parent) :
    QObject { parent },
    _grpcClient(grpcClient),
    _udpClient(udpClient)
{
    _udpClient->setUdpPort(10001);
    _udpClient->startListening();

    connect(_udpClient, &UdpClient::serverInfoReceived, this, &ClientLogic::onServerInfoReceived);
    connect(_grpcClient, &GrpcClient::pingResponseReceived, this, &ClientLogic::onPingServer);

    connect(_grpcClient, &GrpcClient::stopBroadcast, _udpClient, &UdpClient::pauseBroadcast);
    connect(_grpcClient, &GrpcClient::resumeBroadcast, _udpClient, &UdpClient::resumeBroadcast);
}

void ClientLogic::onServerInfoReceived(const QString &serverIp, quint16 serverPort)
{
    _grpcClient->connectToServer(serverIp, serverPort);
    _grpcClient->pingServer();
}

void ClientLogic::onPingServer()
{
    qInfo() << "Ping SUCCESS!!!";
}
