#include <QLineEdit>
#include <QSqlTableModel>
#include <QSqlError>
#include "BjIdDelegate.h"
#include "Dialogs/BjMessageBox.h"
#include "Model/DatabaseManager.h"

BjIdDelegate::BjIdDelegate(QObject *parent):
QStyledItemDelegate(parent)
{

}

QWidget *BjIdDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QLineEdit *editor = new QLineEdit(parent);
	QRegExp regExp("[0-9]{0,50}");
	editor->setValidator(new QRegExpValidator(regExp, parent));
	return editor;
}

void BjIdDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString text = index.model()->data(index, Qt::EditRole).toString();
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	lineEdit->setText(text);
}

void BjIdDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
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

void BjIdDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}