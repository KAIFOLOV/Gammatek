#include "ServerLogic.h"

#include <QTimer>
#include <QDateTime>

ServerLogic::ServerLogic(QPointer<QUdpSocket> udpSocket, QObject *parent) :
    QObject { parent },
    _udpSocket(udpSocket),
    _udpTimer { new QTimer(this) },
    _logFile { new QFile("server_logs.txt", this) }
{
    if (_logFile->open(QIODevice::Append | QIODevice::Text)) {
        _logStream.setDevice(_logFile);
        _logStream << tr("\n\n=== Server Launch at ") << QDateTime::currentDateTime().toString()
                   << tr(" ===\n");
    }

    // connect(_udpSocket, &QUdpSocket::readyRead, this, &ServerLogic::onUdpDataReceived);
    // connect(_udpTimer, &QTimer::timeout, this, &ServerLogic::sendPingResponseToClient);
}

ServerLogic::~ServerLogic()
{
    if (_logFile->isOpen()) {
        _logFile->close();
    }
    qDeleteAll(_clientTimers);
}

void ServerLogic::startBroadcast(const int port)
{
    QByteArray message = QString("Server listening on port: %1").arg(port).toUtf8();
    _udpSocket->writeDatagram(message, QHostAddress::Broadcast, 10001);
    _udpTimer->start(5000);
    appendToLog(tr("UDP broadcast started on port 10001"));
}

void ServerLogic::stopBroadcast()
{
    _udpTimer->stop();
    appendToLog(tr("Broadcasting stopped, awaiting client connections..."));
}

void ServerLogic::onUdpDataReceived()
{
    // QByteArray data;
    // QHostAddress sender;
    // quint16 senderPort;

    // _udpSocket->readDatagram(data.data(), data.size(), &sender, &senderPort);

    // appendToLog(
    //  tr("Received data from %1:%2: %3").arg(sender.toString()).arg(senderPort).arg(data));

    // if (!_clientConnections.contains(sender)) {
    //     _clientConnections[sender] = senderPort;
    //     appendToLog(tr("New client connected: %1:%2").arg(sender.toString()).arg(senderPort));

    //     QTimer *clientPingTimer = new QTimer(this);
    //     _clientTimers[sender] = clientPingTimer;
    //     connect(clientPingTimer, &QTimer::timeout, this, [this, sender, senderPort]() {
    //         sendPingResponseToClient(sender, senderPort);
    //     });

    //     clientPingTimer->start(5000);
    //     stopBroadcast();
    // }
}

void ServerLogic::sendPingResponseToClient(const QHostAddress &clientAddress, const int clientPort)
{
    QByteArray pingResponse = "Ping Response";
    _udpSocket->writeDatagram(pingResponse, clientAddress, clientPort);
    appendToLog(
     tr("Sent Ping Response to client: %1:%2").arg(clientAddress.toString()).arg(clientPort));
}

void ServerLogic::appendToLog(const QString &message)
{
    if (_logStream.device()) {
        _logStream << message << " at " << QDateTime::currentDateTime().toString() << "\n";
    }

    emit logMessage(message);
}
