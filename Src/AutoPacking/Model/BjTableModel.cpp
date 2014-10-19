#include "BjTableModel.h"

BjTableModel::BjTableModel(QObject *parent, QSqlDatabase db):
QSqlTableModel(parent,db)
{
}

Qt::ItemFlags BjTableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;
	Qt::ItemFlags flag = QAbstractItemModel::flags(index);
	//if (index.column() != 3 && index.column() != 4){ 
		flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑
//	}	
	return flag;
}