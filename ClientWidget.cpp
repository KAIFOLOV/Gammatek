#include "ClientWidget.h"

#include "ButtonDelegate.h"

#include <QVBoxLayout>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

ClientWidget::ClientWidget(QPointer<TableModel> tableModel, QWidget *parent) :
    QWidget { parent },
    _tableView(new QTableView(this)),
    _tableModel(tableModel)
{
    _tableView->setModel(_tableModel);
    _tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    auto *buttonDelegate = new ButtonDelegate(this);
    _tableView->setItemDelegateForColumn(3, buttonDelegate);

    connect(buttonDelegate, &ButtonDelegate::buttonClicked, this, &ClientWidget::onButtonClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(_tableView);
}

void ClientWidget::onButtonClicked(const QModelIndex &index, const QString &text)
{
    if (index.isValid()) {
        QPointer<GrpcClient> server = _tableModel->getServer(index.row());
        if (server) {
            if (text == "Connect") {
                server->connectToServer();
            } else if (text == "Disconnect") {
                server->disconnectFromServer();
            }
        }
    }
}
