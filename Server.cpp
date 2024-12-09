#include "ServerWidget.h"
#include "ServerLogic.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPointer<QUdpSocket> udpSocket = new QUdpSocket();

    std::unique_ptr<ServerLogic> serverLogic = std::make_unique<ServerLogic>(udpSocket);
    QPointer<ServerWidget> serverWidget = new ServerWidget();

    QObject::connect(serverLogic.get(), &ServerLogic::logMessage, serverWidget,
                     &ServerWidget::appendToLog);

    QObject::connect(serverWidget, &ServerWidget::startBroadcast, serverLogic.get(),
                     &ServerLogic::startBroadcast);

    QObject::connect(serverWidget, &ServerWidget::stopBroadcast, serverLogic.get(),
                     &ServerLogic::stopBroadcast);

    serverWidget->show();

    return a.exec();
}
