#ifndef MAINTAININGSERVICE_H
#define MAINTAININGSERVICE_H

#include "api.grpc.pb.h"

#include <QDebug>

class MaintainingServiceImpl final : public MaintainingApi::Service
{
public:
    grpc::Status
    Ping(grpc::ServerContext *context, const PingRequest *request, PingResponse *response) override
    {
        qDebug() << "Received ping request from client IP:"
                 << QString::fromStdString(request->clientip());

        response->set_response("Ping Response");

        return grpc::Status::OK;
    }
};

#endif // MAINTAININGSERVICE_H
