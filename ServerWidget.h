#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include "ServerLogic.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QPointer>
#include <QSettings>

#include <memory.h>

class ServerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();

    Q_SIGNAL void startBroadcast(const int port);
    Q_SIGNAL void stopBroadcast();

    Q_SLOT void onStartClicked();
    Q_SLOT void appendToLog(const QString &message);

private:
    QPointer<QLineEdit> _portLineEdit;
    QPointer<QTextEdit> _logTextEdit;
    QPointer<QPushButton> _startButton;
    QPointer<QPushButton> _stopButton;

    QPointer<QSettings> _settings;
};

#endif // SERVERWIDGET_H
