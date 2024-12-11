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

    Q_SIGNAL void connectionStateChanged();
    Q_SIGNAL void lastPingTimeChanged();

    void connectToServer();
    void disconnectFromServer();
    void pingServer();

    QString ip() const;
    quint16 port() const;
    QString lastPingTime() const;
    bool isGrpcConnected() const;

    void setIp(const QString &newIp);
    void setPort(quint16 newPort);

private:
    std::shared_ptr<grpc::Channel> _channel;
    std::unique_ptr<MaintainingApi::MaintainingApi::Stub> _stub;

    /// Счетчик попыток пинга
    int _pingFailureCount = 0;
    /// Флаг коннекта к серверу
    bool _isGrpcConnected = false;
    /// Таймер для запроса пинга
    QPointer<QTimer> _pingTimer { nullptr };
    /// ip сервера
    QString _ip {};
    /// порт сервера
    quint16 _port {};
    /// Последнее время пинга
    QString _lastPingTime { "-" };
};

#endif // GRPCCLIENT_H
