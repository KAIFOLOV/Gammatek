#include "ButtonDelegate.h"

#include <QPushButton>
#include <QPainter>

ButtonDelegate::ButtonDelegate(QObject *parent) : QStyledItemDelegate(parent)
{}

QWidget *ButtonDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    Q_UNUSED(option);

    auto button = new QPushButton(parent);
    button->setText(index.data(Qt::DisplayRole).toString());
    connect(button, &QPushButton::clicked, this, [index, this, button]() {
        emit buttonClicked(index, button->text());
    });
    return button;
}

void ButtonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *button = static_cast<QPushButton *>(editor);
    QString buttonText = index.data(Qt::DisplayRole).toString();
    button->setText(buttonText);
}

void ButtonDelegate::setModelData(QWidget *editor,
                                  QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(model);
    Q_UNUSED(index);
    // Для кнопки не требуется сохранять данные обратно в модель
}

void ButtonDelegate::updateEditorGeometry(QWidget *editor,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex & /*index*/) const
{
    editor->setGeometry(option.rect);
}

void ButtonDelegate::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    QPushButton button;
    button.setText(index.data(Qt::DisplayRole).toString());
    button.resize(option.rect.size());

    QPixmap pixmap(button.size());
    button.render(&pixmap);

    painter->drawPixmap(option.rect.topLeft(), pixmap);
}
