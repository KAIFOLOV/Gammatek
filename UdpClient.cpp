#include "UdpClient.h"

UdpClient::UdpClient(QObject *parent) : QObject(parent), _udpSocket { new QUdpSocket(this) }
{
    connect(_udpSocket, &QUdpSocket::readyRead, this, &UdpClient::onDatagramReceived);
}

void UdpClient::startListening(const quint16 port)
{
    if (!_udpSocket->bind(QHostAddress::LocalHost, port, QUdpSocket::ShareAddress)) {
        qCritical() << "Failed to bind UDP socket to port" << port;
        return;
    }

    qInfo() << "Listening on UDP port" << port;
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
        qInfo() << "Received datagram:" << message;

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
