#include "UdpClient.h"

UdpClient::UdpClient(QObject *parent) : QObject(parent), _udpSocket { new QUdpSocket(this) }
{
    connect(_udpSocket, &QUdpSocket::readyRead, this, &UdpClient::onDatagramReceived);
}

void UdpClient::startListening()
{
    if (!_udpSocket->bind(QHostAddress::Any, _udpPort, QUdpSocket::ShareAddress)) {
        qCritical() << "Failed to bind UDP socket to port" << _udpPort;
        return;
    }

    qInfo() << "Listening on UDP port" << _udpPort;
}

void UdpClient::onDatagramReceived()
{
    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(_udpSocket->pendingDatagramSize()));

        QHostAddress sender;
        quint16 senderPort;
        _udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QString message = QString::fromUtf8(datagram);

        QStringList parts = message.split(":");
        if (parts.size() == 2) {
            QString serverIp = parts[0];
            quint16 serverPort = parts[1].toUInt();

            emit serverInfoReceived(serverIp, serverPort);
        } else {
            qWarning() << "Invalid datagram format received!";
        }
    }
}

void UdpClient::pauseBroadcast()
{
    qInfo() << "Pausing broadcast for";
    _udpSocket->close();
}

void UdpClient::resumeBroadcast()
{
    qInfo() << "Resuming broadcast.";
    delete _udpSocket;
    _udpSocket = new QUdpSocket(this);

    connect(_udpSocket, &QUdpSocket::readyRead, this, &UdpClient::onDatagramReceived);
    startListening();
}

void UdpClient::setUdpPort(const quint16 newUdpPort)
{
    _udpPort = newUdpPort;
}
