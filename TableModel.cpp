#include "TableModel.h"
#include <QPushButton>
#include <QTimer>
#include <QTime>

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{
    addServer("172.16.1.1");
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _servers.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4; // IP, Last Ping Time, Status, Action
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const ServerData &server = _servers[index.row()];
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return server.adress;
        case 1:
            return server.lastPingTime;
        case 2:
            return server.status;
        case 3:
            return server.status == "Offline" ? "Connect" : "Disconnect";
        }
    }

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "IP Address";
        case 1:
            return "Last Ping Time";
        case 2:
            return "Status";
        case 3:
            return "Action";
        default:
            return QVariant();
        }
    }
    return QVariant();
}

void TableModel::addServer(const QString &adress)
{
    beginInsertRows(QModelIndex(), _servers.size(), _servers.size());
    ServerData server;
    server.adress = adress;
    server.status = "Offline";
    server.lastPingTime = "-";
    server.pingTimer = nullptr;
    _servers.append(server);
    endInsertRows();
}

void TableModel::startPing(const int row)
{
    ServerData &server = _servers[row];
    if (server.pingTimer)
        return; // Если уже пингуется, ничего не делаем

    // Создаем таймер для пинга каждые 5 секунд
    server.pingTimer = new QTimer(this);
    server.pingTimer->setInterval(5000);
    connect(server.pingTimer, &QTimer::timeout, this, [this, row]() {
        pingServer(row);
    });
    server.pingTimer->start();
}

void TableModel::stopPing(const int row)
{
    ServerData &server = _servers[row];
    if (server.pingTimer) {
        server.pingTimer->stop();
        delete server.pingTimer;
        server.pingTimer = nullptr;
    }
}

void TableModel::pingServer(const int row)
{
    ServerData &server = _servers[row];

    // Пример: обновляем время последнего пинга
    server.lastPingTime = QTime::currentTime().toString("hh:mm");

    // Здесь нужно добавить вызов функции Ping через gRPC или другую технологию

    // Обновляем данные в таблице (только колонку с временем пинга)
    emit dataChanged(index(row, 1), index(row, 1));
}

void TableModel::toggleConnectDisconnect(int row, QPushButton *button)
{
    ServerData &server = _servers[row];

    if (server.status == "Offline") {
        startPing(row);
        server.status = "Online";
        button->setText("Disconnect");
    } else {
        stopPing(row);
        server.status = "Offline";
        button->setText("Connect");
    }

    emit dataChanged(index(row, 2), index(row, 2)); // Обновить статус
}
