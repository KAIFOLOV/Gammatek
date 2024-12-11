#ifndef GRPCSERVER_H
#define GRPCSERVER_H

#include <QString>
#include <QThread>

#include "api.grpc.pb.h"

#include <grpcpp/grpcpp.h>

class MaintainingServiceImpl final : public MaintainingApi::Service
{
public:
    grpc::Status
    Ping(grpc::ServerContext *context, const PingRequest *request, PingResponse *response) override;
};

class GrpcServer : public QThread
{
    Q_OBJECT

public:
    GrpcServer();

    Q_SIGNAL void logMessage(const QString &message);

    void setServerIp(const QString &newServerIp);
    void setServerPort(quint16 newServerPort);

protected:
    void run() override;

private:
    QString _serverIp { "127.0.0.1" };
    quint16 _serverPort { 50001 };

    std::shared_ptr<grpc::Channel> _channel;
    std::unique_ptr<MaintainingApi::Stub> _stub;
    std::unique_ptr<grpc::Server> _server;
};

#endif // GRPCSERVER_H
