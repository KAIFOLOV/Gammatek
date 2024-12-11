#ifndef CLIENTLOGIC_H
#define CLIENTLOGIC_H

#include "TableModel.h"
#include "UdpClient.h"

#include <QObject>

/// Бизнесс логика
class ClientLogic : public QObject
{
    Q_OBJECT
public:
    explicit ClientLogic(QPointer<UdpClient> udpClient,
                         QPointer<TableModel> model,
                         QObject *parent = nullptr);

    Q_SIGNAL void stopBroadcast();
    Q_SIGNAL void resumeBroadcast();

    Q_SLOT void onServerInfoReceived(const QString &serverIp, const quint16 serverPort);

private:
    QPointer<UdpClient> _udpClient;
    QPointer<TableModel> _model;
};

#endif // CLIENTLOGIC_H
