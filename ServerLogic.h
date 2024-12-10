#ifndef SERVERLOGIC_H
#define SERVERLOGIC_H

#include "GrpcServer.h"
#include "UdpServer.h"

#include <QObject>
#include <QUdpSocket>
#include <QFile>
#include <QPointer>

class ServerLogic : public QObject
{
    Q_OBJECT
public:
    explicit ServerLogic(QPointer<GrpcServer> grpcServer,
                         QPointer<UdpServer> udpServer,
                         QObject *parent = nullptr);
    ~ServerLogic();

    Q_SIGNAL void logMessage(const QString &message);
    Q_SLOT void appendToLog(const QString &message);

    void startGrpcServer();
    void stopGrpcServer();

    void startUdpBroadcast(const quint16 serverPort);
    void stopUdpBroadcast();

private:
    QPointer<GrpcServer> _grpcServer;
    QPointer<UdpServer> _udpServer;

    QPointer<QFile> _logFile;
    QTextStream _logStream;

    bool _isBroadcasting = false;
};

#endif // SERVERLOGIC_H
