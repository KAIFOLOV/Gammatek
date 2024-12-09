#ifndef SERVERLOGIC_H
#define SERVERLOGIC_H

#include <QObject>
#include <QUdpSocket>
#include <QFile>
#include <QPointer>

class ServerLogic : public QObject
{
    Q_OBJECT
public:
    explicit ServerLogic(QPointer<QUdpSocket> udpSocket, QObject *parent = nullptr);
    ~ServerLogic();

    Q_SLOT void startBroadcast(const int port);
    Q_SLOT void stopBroadcast();

    void onUdpDataReceived();
    void sendPingResponseToClient(const QHostAddress &clientAddress, const int clientPort);

    Q_SIGNAL void logMessage(const QString &message);

    void setUdpSocket(QPointer<QUdpSocket> newUdpSocket);

private:
    void appendToLog(const QString &message);

    QPointer<QUdpSocket> _udpSocket;
    QPointer<QTimer> _udpTimer;

    QMap<QHostAddress, int> _clientConnections; // Подключенные клиенты
    QMap<QHostAddress, QPointer<QTimer>> _clientTimers; // Таймеры для пинга

    QPointer<QFile> _logFile;
    QTextStream _logStream;
};

#endif // SERVERLOGIC_H
