#include "ClientLogic.h"

ClientLogic::ClientLogic(QPointer<UdpClient> udpClient,
                         QPointer<TableModel> model,
                         QObject *parent) :
    QObject { parent },
    _udpClient(udpClient),
    _model(model)
{
    _udpClient->setUdpPort(10001);
    _udpClient->startListening();

    connect(_udpClient, &UdpClient::serverInfoReceived, this, &ClientLogic::onServerInfoReceived);
}

void ClientLogic::onServerInfoReceived(const QString &serverIp, quint16 serverPort)
{
    _model->addServer(serverIp, serverPort);
}
