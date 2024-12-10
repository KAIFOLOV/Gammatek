#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QPointer>
#include <QTimer>

class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr);

    Q_SIGNAL void logMessage(const QString &message);

    Q_SLOT void sendBroadcastMessage();

    void startBroadcast(const quint16 serverPort);
    void stopBroadcast();

    void setBroadcastPort(const quint16 newBroadcastPort);

    void setServerIp(const QString &newServerIp);

private:
    QPointer<QUdpSocket> _udpSocket { nullptr };
    QPointer<QTimer> _broadcastTimer { nullptr };

    QString _serverIp = "127.0.0.1";
    quint16 _broadcastPort = 10001;
    quint16 _serverPort = 50051;

    bool _isBroadcasting = false;
};

#endif // UDPSERVER_H
