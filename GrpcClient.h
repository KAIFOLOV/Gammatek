#ifndef GRPCCLIENT_H
#define GRPCCLIENT_H

#include <QObject>
#include <QPointer>
#include <QTimer>

#include <grpcpp/grpcpp.h>
#include "api.grpc.pb.h"

class GrpcClient : public QObject
{
    Q_OBJECT

public:
    explicit GrpcClient(QObject *parent = nullptr);

    Q_SIGNAL void pingResponseReceived(const QString &response);
    Q_SIGNAL void stopBroadcast();
    Q_SIGNAL void resumeBroadcast();

    void connectToServer(const QString &serverIp, const quint16 serverPort);
    void pingServer();

private:
    std::shared_ptr<grpc::Channel> _channel;
    std::unique_ptr<MaintainingApi::MaintainingApi::Stub> _stub;
    /// Счетчик попыток пинга
    int _pingFailureCount = 0;
    /// Флаг коннекта к серверу
    bool _isGrpcConnected = false;
    /// Таймер для запроса пинга
    QPointer<QTimer> _pingTimer;
};

#endif // GRPCCLIENT_H
