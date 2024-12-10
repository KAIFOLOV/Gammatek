#ifndef CLIENTLOGIC_H
#define CLIENTLOGIC_H

#include <QObject>

class ClientLogic : public QObject
{
    Q_OBJECT
public:
    explicit ClientLogic(QObject *parent = nullptr);

signals:
};

#endif // CLIENTLOGIC_H
