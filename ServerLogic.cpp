#include "ServerLogic.h"

#include "api.pb.h"
#include "api.grpc.pb.h"

#include <QTimer>
#include <QDateTime>
#include <QNetworkInterface>

ServerLogic::ServerLogic(QPointer<QUdpSocket> udpSocket, GrpcServer *grpcServer, QObject *parent) :
    QObject { parent },
    _udpSocket(udpSocket),
    _broadcastTimer { new QTimer(this) },
    _grpcServer(grpcServer),
    _logFile { new QFile("server_logs.txt", this) }
{
    if (_logFile->open(QIODevice::Append | QIODevice::Text)) {
        _logStream.setDevice(_logFile);
        _logStream << tr("\n\n=== Server Launch at ") << QDateTime::currentDateTime().toString()
                   << tr(" ===\n");
    }

    _serverIp = getLocalIPAddress();

    connect(_broadcastTimer, &QTimer::timeout, this, &ServerLogic::sendBroadcastMessage);
}

ServerLogic::~ServerLogic()
{
    if (_logFile->isOpen()) {
        _logFile->close();
    }
    qDeleteAll(_clientTimers);
}

void ServerLogic::startBroadcast(const quint16 port)
{
    appendToLog(tr("Starting broadcast..."));

    if (_isBroadcasting) {
        appendToLog(tr("Broadcast is already active on port %1").arg(_broadcastPort));
        return;
    }

    if (!_udpSocket->bind(QHostAddress::LocalHost, _broadcastPort, QUdpSocket::ShareAddress)) {
        appendToLog(tr("Failed to bind to port %1").arg(_broadcastPort));
        return;
    }

    _serverPort = port;

    // GRPC
    {
        _grpcServer->setServerIp(_serverIp);
        _grpcServer->setServerPort(_serverPort);
        _grpcServer->start();
    }

    _broadcastTimer->start(1000);
    _isBroadcasting = true;

    appendToLog(tr("Broadcast started on port %1").arg(_broadcastPort));
    appendToLog(tr("Server IP: %1, Server Port: %2").arg(_serverIp, QString::number(port)));
}

void ServerLogic::stopBroadcast()
{
    if (!_isBroadcasting) {
        appendToLog(tr("Broadcast is not active, cannot stop."));
        return;
    }

    if (_broadcastTimer && _broadcastTimer->isActive()) {
        _broadcastTimer->stop();
    }

    _udpSocket->close();
    _isBroadcasting = false;

    appendToLog(tr("Broadcast stopped"));
}

void ServerLogic::sendBroadcastMessage()
{
    QByteArray message = QString("%1:%2").arg(_serverIp).arg(_serverPort).toUtf8();
    qDebug() << "Broadcasting message:" << message;

    _udpSocket->writeDatagram(message, QHostAddress::Broadcast, _broadcastPort);

    if (_udpSocket->writeDatagram(message, QHostAddress::Broadcast, _broadcastPort) == -1) {
        appendToLog(tr("Failed to send broadcast message"));
    } else {
        appendToLog(tr("Broadcast message sent: %1").arg(QString(message)));
    }
}

void ServerLogic::sendPingResponseToClient(const QHostAddress &clientAddress, quint16 clientPort)
{
    grpc::ChannelArguments channel_args;
    std::shared_ptr<grpc::Channel> channel =
     grpc::CreateCustomChannel("localhost:50051", grpc::InsecureChannelCredentials(), channel_args);

    std::unique_ptr<MaintainingApi::Stub> stub = MaintainingApi::NewStub(channel);

    PingRequest request;
    request.set_clientip(clientAddress.toString().toStdString());

    PingResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub->Ping(&context, request, &response);

    if (status.ok()) {
        qInfo() << "Received Ping Response: " << QString::fromStdString(response.response());
        appendToLog(tr("Received Ping Response from client: %1:%2")
                     .arg(clientAddress.toString())
                     .arg(clientPort));
    } else {
        appendToLog(tr("Failed to send Ping Response to client"));
    }
}

void ServerLogic::appendToLog(const QString &message)
{
    if (_logStream.device()) {
        _logStream << message << " at " << QDateTime::currentDateTime().toString() << "\n";
    }

    emit logMessage(message);
}

QString ServerLogic::getLocalIPAddress()
{
    // for (const QNetworkInterface &interface : QNetworkInterface::allInterfaces()) {
    //     // Пропускаем интерфейсы, которые отключены или не работают
    //     if (interface.flags() & QNetworkInterface::IsRunning
    //         && !(interface.flags() & QNetworkInterface::IsLoopBack)) {
    //         for (const QNetworkAddressEntry &entry : interface.addressEntries()) {
    //             if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
    //                 return entry.ip().toString();
    //             }
    //         }
    //     }
    // }
    return "172.16.2.77";
}
