#include "TableModel.h"
#include <QPushButton>
#include <QTimer>
#include <QTime>

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _servers.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const auto server = _servers[index.row()];
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return QString(server->ip() % ":" % QString::number(server->port()));
        case 1:
            return server->lastPingTime();
        case 2:
            return server->isGrpcConnected() ? "ON" : "OFF";
        case 3:
            return server->isGrpcConnected() ? "Disconnect" : "Connect";
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

void TableModel::addServer(const QString &ip, const quint16 port)
{
    for (const auto &server : _servers) {
        if (server->ip() == ip && server->port() == port) {
            qInfo() << "Server with IP" << ip << "and port" << port << "already exists!";
            return;
        }
    }

    beginInsertRows(QModelIndex(), _servers.size(), _servers.size());

    QPointer<GrpcDevice> server = new GrpcDevice();
    server->setIp(ip);
    server->setPort(port);
    _servers.append(server);

    // Можно и dataChange
    connect(server.data(), &GrpcDevice::connectionStateChanged, this, [this]() {
        emit layoutChanged();
    });

    connect(server.data(), &GrpcDevice::lastPingTimeChanged, this, [this]() {
        emit layoutChanged();
    });

    endInsertRows();
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if (index.column() == 3) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

QPointer<GrpcDevice> TableModel::getServer(const int row) const
{
    if (row >= 0 && row < _servers.size()) {
        return _servers.at(row);
    }
    return nullptr;
}
