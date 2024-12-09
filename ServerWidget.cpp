#include "ServerWidget.h"

#include <QVBoxLayout>
#include <QLabel>

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget { parent },
    _settings { new QSettings("Gammatek", "ServerApp", this) }
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    _logTextEdit = new QTextEdit(this);
    _logTextEdit->setReadOnly(true);
    mainLayout->addWidget(_logTextEdit);

    {
        QHBoxLayout *portLayout = new QHBoxLayout(this);

        uint savedPort = _settings->value("server_port", 8080).toUInt();
        _portLineEdit = new QLineEdit(this);
        _portLineEdit->setPlaceholderText(tr("Enter port"));

        _portLineEdit->setText(QString::number(savedPort));

        portLayout->addWidget(new QLabel(tr("Port:")));
        portLayout->addWidget(_portLineEdit);

        mainLayout->addLayout(portLayout);
    }

    // control buttons
    {
        QHBoxLayout *buttonLayout = new QHBoxLayout(this);

        _startButton = new QPushButton(tr("Start"), this);
        buttonLayout->addWidget(_startButton);

        _stopButton = new QPushButton(tr("Stop"), this);
        buttonLayout->addWidget(_stopButton);

        mainLayout->addLayout(buttonLayout);
    }

    setLayout(mainLayout);

    qInfo() << "ServerWidget constructor called!";

    connect(_startButton, &QPushButton::clicked, this, &ServerWidget::onStartClicked);
    connect(_stopButton, &QPushButton::clicked, this, &ServerWidget::stopBroadcast);
}

ServerWidget::~ServerWidget()
{}

void ServerWidget::onStartClicked()
{
    bool ok;
    const int port = _portLineEdit->text().toInt(&ok);
    if (!ok || port <= 0 || port > 65535) {
        appendToLog(tr("Invalid port entered. Please enter a valid port number"));
        return;
    }

    _settings->setValue("server_port", port);

    appendToLog(tr("Starting server..."));

    emit startBroadcast(port);

    appendToLog(tr("Server successfully started on port: ") + QString::number(port));
}

void ServerWidget::appendToLog(const QString &message)
{
    _logTextEdit->append(message);
}