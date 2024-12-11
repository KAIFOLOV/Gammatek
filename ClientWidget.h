#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QPointer>
#include <QTableView>
#include <QMouseEvent>

#include "TableModel.h"

class ClientWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClientWidget(QPointer<TableModel> tableModel, QWidget *parent = nullptr);

    Q_SLOT void onButtonClicked(const QModelIndex &index, const QString &text);

private:
    QPointer<QTableView> _tableView { nullptr };
    QPointer<TableModel> _tableModel { nullptr };
};

#endif // CLIENTWIDGET_H
