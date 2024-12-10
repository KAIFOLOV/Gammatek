#include "UdpServer.h"

UdpServer::UdpServer(QObject *parent) :
    QObject { parent },
    _udpSocket { new QUdpSocket(this) },
    _broadcastTimer { new QTimer(this) }
{}

void UdpServer::startBroadcast(const quint16 serverPort)
{
    if (_isBroadcasting) {
        emit logMessage(tr("Broadcast is already active on port %1").arg(_broadcastPort));
        return;
    }

    _serverPort = serverPort;

    if (!_udpSocket->bind(QHostAddress::LocalHost, _broadcastPort, QUdpSocket::ShareAddress)) {
        emit logMessage(tr("Failed to bind to port %1").arg(_broadcastPort));
        return;
    }

    connect(_broadcastTimer, &QTimer::timeout, this, &UdpServer::sendBroadcastMessage);
    _broadcastTimer->start(3000);
    _isBroadcasting = true;

    emit logMessage(tr("Broadcast started on port %1").arg(_broadcastPort));
}

void UdpServer::stopBroadcast()
{
    if (!_isBroadcasting) {
        emit logMessage(tr("Broadcast is not active, cannot stop."));
        return;
    }

    _broadcastTimer->stop();
    _udpSocket->close();
    _isBroadcasting = false;

    emit logMessage(tr("Broadcast stopped"));
}

void UdpServer::setBroadcastPort(const quint16 newBroadcastPort)
{
    _broadcastPort = newBroadcastPort;
}

void UdpServer::setServerIp(const QString &newServerIp)
{
    _serverIp = newServerIp;
}

void UdpServer::sendBroadcastMessage()
{
    QByteArray message = QString("%1:%2").arg(_serverIp).arg(_serverPort).toUtf8();
    if (_udpSocket->writeDatagram(message, QHostAddress::Broadcast, _broadcastPort) == -1) {
        emit logMessage(tr("Failed to send broadcast message"));
    } else {
        emit logMessage(tr("Broadcast message sent: %1").arg(QString(message)));
    }
}
