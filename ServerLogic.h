#ifndef SERVERLOGIC_H
#define SERVERLOGIC_H

#include "GrpcServer.h"

#include <QObject>
#include <QUdpSocket>
#include <QFile>
#include <QPointer>

class ServerLogic : public QObject
{
    Q_OBJECT
public:
    explicit ServerLogic(QPointer<QUdpSocket> udpSocket,
                         GrpcServer * grpcServer,
                         QObject *parent = nullptr);
    ~ServerLogic();

    Q_SIGNAL void logMessage(const QString &message);

    Q_SLOT void startBroadcast(const quint16 port);
    Q_SLOT void stopBroadcast();
    Q_SLOT void sendBroadcastMessage();

    void sendPingResponseToClient(const QHostAddress &clientAddress, const quint16 clientPort);

    void setUdpSocket(QPointer<QUdpSocket> newUdpSocket);

private:
    void appendToLog(const QString &message);
    QString getLocalIPAddress();

    QPointer<QUdpSocket> _udpSocket;
    QPointer<QTimer> _broadcastTimer;
    GrpcServer *_grpcServer;

    QHash<QHostAddress, quint16> _clientConnections; // Подключенные клиенты
    QHash<QHostAddress, QPointer<QTimer>> _clientTimers; // Таймеры для пинга

    QPointer<QFile> _logFile;
    QTextStream _logStream;

    bool _isBroadcasting = false;
    quint16 _broadcastPort = 10001;
    QString _serverIp = "127.0.0.1";
    quint16 _serverPort = 50051;
};

#endif // SERVERLOGIC_H
