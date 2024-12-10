#include "ClientLogic.h"
#include "ClientWidget.h"
#include "UdpClient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPointer<UdpClient> udpClient = new UdpClient();
    QPointer<GrpcClient> grpcClient = new GrpcClient();

    QPointer<TableModel> model = new TableModel();
    QPointer<ClientWidget> widget = new ClientWidget(model);
    widget->setMinimumHeight(300);
    widget->setMinimumWidth(500);

    std::unique_ptr<ClientLogic> serverLogic = std::make_unique<ClientLogic>(grpcClient, udpClient);

    widget->show();

    return a.exec();
}
