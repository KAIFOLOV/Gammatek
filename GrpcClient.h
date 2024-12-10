#ifndef GRPCCLIENT_H
#define GRPCCLIENT_H

#include <QObject>

#include <grpcpp/grpcpp.h>
#include "api.grpc.pb.h"

class GrpcClient : public QObject
{
    Q_OBJECT

public:
    explicit GrpcClient(QObject *parent = nullptr);

    Q_SIGNAL void pingResponseReceived(const QString &response);

    void pingServer();
    void connectToServer(const QString &serverIp, quint16 serverPort);

private:
    std::shared_ptr<grpc::Channel> _channel;
    std::unique_ptr<MaintainingApi::MaintainingApi::Stub> _stub;
    bool _isGrpcConnected = false;
};

#endif // GRPCCLIENT_H
