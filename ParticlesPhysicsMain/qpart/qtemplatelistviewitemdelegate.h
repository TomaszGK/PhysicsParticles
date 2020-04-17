#pragma once
#include <QStyledItemDelegate>
#include <QFileInfo>
#include <QApplication>

class QTemplateListViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:

    explicit QTemplateListViewItemDelegate( QObject *parent = nullptr );

protected:

    void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        QFileInfo fileinfo(opt.text);
        opt.text = fileinfo.completeBaseName();
        QStyle* style = opt.widget ? opt.widget->style() : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
    }

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QSize result = QStyledItemDelegate::sizeHint(option, index);
        return result;
    }

};


