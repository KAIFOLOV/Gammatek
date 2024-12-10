#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QPushButton>
#include <QPointer>

struct ServerData
{
    QString adress;
    QString lastPingTime;
    QString status;
    QPointer<QTimer> pingTimer;
};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = nullptr);

    Q_SLOT void toggleConnectDisconnect(const int row, QPushButton *button);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void addServer(const QString &adress);
    void startPing(const int row);
    void stopPing(const int row);
    void pingServer(const int row);

private:
    QList<ServerData> _servers;
};

#endif // TABLEMODEL_H
