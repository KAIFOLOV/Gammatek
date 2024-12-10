#include "ServerLogic.h"

#include <QDateTime>

ServerLogic::ServerLogic(QPointer<GrpcServer> grpcServer,
                         QPointer<UdpServer> udpServer,
                         QObject *parent) :
    QObject { parent },
    _grpcServer(grpcServer),
    _udpServer(udpServer),
    _logFile { new QFile("server_logs.txt", this) }
{
    if (_logFile->open(QIODevice::Append | QIODevice::Text)) {
        _logStream.setDevice(_logFile);
        _logStream << tr("\n\n=== Server Launch at ") << QDateTime::currentDateTime().toString()
                   << tr(" ===\n");
    }
}

ServerLogic::~ServerLogic()
{
    if (_logFile->isOpen()) {
        _logFile->close();
    }
}

void ServerLogic::startGrpcServer()
{
    // Запускаем gRPC сервер
    _grpcServer->start();

    appendToLog(tr("gRPC server started"));
}

void ServerLogic::stopGrpcServer()
{
    appendToLog(tr("Stopping gRPC server"));

    _grpcServer->quit();

    appendToLog(tr("gRPC server stopped"));
}

void ServerLogic::startUdpBroadcast(const quint16 serverPort)
{
    qInfo() << "startUdpBroadcast()";
    _udpServer->startBroadcast(serverPort);
}

void ServerLogic::stopUdpBroadcast()
{
    _udpServer->stopBroadcast();
}

void ServerLogic::appendToLog(const QString &message)
{
    if (_logStream.device()) {
        _logStream << message << " at " << QDateTime::currentDateTime().toString() << "\n";
    }

    emit logMessage(message);
}
