#ifndef CLIENTLOGIC_H
#define CLIENTLOGIC_H

#include "GrpcClient.h"
#include "UdpClient.h"

#include <QObject>

class ClientLogic : public QObject
{
    Q_OBJECT
public:
    explicit ClientLogic(QPointer<GrpcClient> grpcClient,
                         QPointer<UdpClient> udpClient,
                         QObject *parent = nullptr);

    Q_SIGNAL void stopBroadcast();
    Q_SIGNAL void resumeBroadcast();

    Q_SLOT void onServerInfoReceived(const QString &serverIp, quint16 serverPort);
    Q_SLOT void onPingServer();

private:
    QPointer<GrpcClient> _grpcClient;
    QPointer<UdpClient> _udpClient;
};

#endif // CLIENTLOGIC_H
