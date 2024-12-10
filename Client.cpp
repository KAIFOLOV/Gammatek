#include "ClientLogic.h"
#include "UdpClient.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QPointer<UdpClient> udpClient = new UdpClient();
    QPointer<GrpcClient> grpcClient = new GrpcClient();

    std::unique_ptr<ClientLogic> serverLogic = std::make_unique<ClientLogic>(grpcClient, udpClient);

    return a.exec();
}
