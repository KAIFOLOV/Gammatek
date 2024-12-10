#include "ServerWidget.h"
#include "ServerLogic.h"
#include "GrpcServer.h"

#include <QApplication>
#include <QPointer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPointer<GrpcServer> grpcServer = new GrpcServer();
    QPointer<UdpServer> udpServer = new UdpServer();

    QPointer<ServerWidget> serverWidget = new ServerWidget();

    std::unique_ptr<ServerLogic> serverLogic = std::make_unique<ServerLogic>(grpcServer, udpServer);

    QObject::connect(grpcServer, &GrpcServer::logMessage, serverLogic.get(),
                     &ServerLogic::appendToLog);

    QObject::connect(udpServer, &UdpServer::logMessage, serverLogic.get(),
                     &ServerLogic::appendToLog);

    QObject::connect(serverLogic.get(), &ServerLogic::logMessage, serverWidget,
                     &ServerWidget::appendToLog);

    QObject::connect(serverWidget, &ServerWidget::startBroadcast, serverLogic.get(),
                     &ServerLogic::startUdpBroadcast);

    QObject::connect(serverWidget, &ServerWidget::startGrpc, serverLogic.get(),
                     &ServerLogic::startGrpcServer);

    QObject::connect(serverWidget, &ServerWidget::stopBroadcast, serverLogic.get(),
                     &ServerLogic::stopUdpBroadcast);

    serverWidget->show();

    return a.exec();
}
