#include <QLineEdit>
#include "BjEditDelegate.h"

BjEditDelegate::BjEditDelegate(QObject *parent) :
QStyledItemDelegate(parent)
{

}

QWidget *BjEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	//QLineEdit *editor = new QLineEdit(parent);
	//return editor;
	return NULL;
}

void BjEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void BjEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

void BjEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	//editor->setGeometry(option.rect);
}