#ifndef GRPCSERVER_H
#define GRPCSERVER_H

#include <QString>
#include <QThread>

#include <grpcpp/grpcpp.h>

class GrpcServer : public QThread
{
    Q_OBJECT

public:
    GrpcServer();

    void setServerIp(const QString &newServerIp);
    void setServerPort(quint16 newServerPort);

protected:
    void run() override;

private:
    QString _serverIp;
    quint16 _serverPort;
    std::unique_ptr<grpc::Server> _server;
};

#endif // GRPCSERVER_H
