#include "GrpcServer.h"

#include "MaintainingService.h"

#include <QDebug>

GrpcServer::GrpcServer()
{}

void GrpcServer::run()
{
    std::string server_address = _serverIp.toStdString() + ":" + std::to_string(_serverPort);

    MaintainingServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    _server = builder.BuildAndStart();
    qInfo() << "Server listening on" << QString::fromStdString(server_address);

    _server->Wait();
}

void GrpcServer::setServerIp(const QString &newServerIp)
{
    _serverIp = newServerIp;
}

void GrpcServer::setServerPort(quint16 newServerPort)
{
    _serverPort = newServerPort;
}
