#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "GrpcClient.h"

#include <QAbstractTableModel>
#include <QPushButton>
#include <QPointer>

struct Server
{
    QString ip;
    quint16 port;
    QString lastPingTime;
    QString status;
    QPointer<QTimer> pingTimer;
};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void addServer(const QString &ip, const quint16 port);

    QPointer<GrpcClient> getServer(const int row) const;

private:
    QList<QPointer<GrpcClient>> _servers;
};

#endif // TABLEMODEL_H
