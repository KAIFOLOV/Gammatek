#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QPointer>
#include <QUdpSocket>

class UdpClient : public QObject
{
    Q_OBJECT
public:
    explicit UdpClient(QObject *parent = nullptr);

    Q_SIGNAL void serverInfoReceived(const QString &serverIp, quint16 serverPort);

    Q_SLOT void onDatagramReceived();
    Q_SLOT void resumeBroadcast();

    void startListening();
    void pauseBroadcast();

    void setUdpPort(const quint16 newUdpPort);

private:
    quint16 _udpPort;
    QPointer<QUdpSocket> _udpSocket;
};

#endif // UDPCLIENT_H
