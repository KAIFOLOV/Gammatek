#include "ClientWidget.h"

#include <QVBoxLayout>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

ClientWidget::ClientWidget(QPointer<TableModel> tableModel, QWidget *parent) :
    QWidget { parent },
    _tableView(new QTableView()),
    _tableModel(tableModel)
{
    _tableView = new QTableView();
    _tableView->setModel(_tableModel);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(_tableView);
}

