#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSqlError>
#include <QLineEdit>
#include <QApplication>
#include <QPushButton>
#include <QProgressBar>
#include "BjTableDelegate.h"
#include "DatabaseManager.h"
#include "Dialogs/BjMessageBox.h"

BjTableDelegate::BjTableDelegate(QObject *parent) :
QStyledItemDelegate(parent)
{
}

QWidget *BjTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QLineEdit *editor = new QLineEdit(parent);
	return editor;
}

void BjTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString text = index.model()->data(index, Qt::EditRole).toString();
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	lineEdit->setText(text);
}

void BjTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	QString text = lineEdit->text().trimmed();
	QSqlTableModel *ptableModel = DatabaseManager::GetInstance()->GetTableModel();
	if (!ptableModel->setData(index, text, Qt::EditRole)){
		BjMessageBox::warning(NULL, QStringLiteral("tableModel"), QStringLiteral("数据库错误: %1").arg(ptableModel->lastError().text()));
		ptableModel->revertAll();//如果不删除，则撤销; 
	}
	if (!ptableModel->submitAll()){
		BjMessageBox::warning(NULL, QStringLiteral("tableModel"), QStringLiteral("数据库错误: %1").arg(ptableModel->lastError().text()));
		ptableModel->revertAll();//如果不删除，则撤销;
	}
}

void BjTableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

void BjTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex & index) const
{
	QStyledItemDelegate::paint(painter, option, index);
}