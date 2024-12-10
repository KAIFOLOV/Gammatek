#include "ClientLogic.h"
#include "UdpClient.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QPointer<UdpClient> udpClient = new UdpClient();

    // std::unique_ptr<ClientLogic> serverLogic = std::make_unique<ClientLogic>(udpClient);
    // udpClient.startListening(10001);

    return a.exec();
}
