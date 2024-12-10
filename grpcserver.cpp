#include "GrpcServer.h"

#include <QDebug>

grpc::Status MaintainingServiceImpl::Ping(grpc::ServerContext *context,
                                          const PingRequest *request,
                                          PingResponse *response)
{
    std::string client_ip = context->peer();
    qInfo() << "Send Ping Responce to client:" << QString::fromStdString(client_ip);

    std::string responseMessage = "1";

    response->set_response(responseMessage);

    return grpc::Status::OK;
}

GrpcServer::GrpcServer()
{}

void GrpcServer::run()
{
    emit logMessage(tr("Starting gRPC server at %1:%2").arg(_serverIp).arg(_serverPort));

    std::string server_address = _serverIp.toStdString() + ":" + std::to_string(_serverPort);

    MaintainingServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    _server = builder.BuildAndStart();
    qInfo() << "gRPC Server listening on" << QString::fromStdString(server_address);

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
