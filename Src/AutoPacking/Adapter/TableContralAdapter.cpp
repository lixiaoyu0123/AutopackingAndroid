#include "TableContralAdapter.h"
#include "Model/DatabaseManager.h"
#include "Dialogs/EditTable.h"

void TableContralAdapter::AddRow()
{
	DatabaseManager::GetInstance()->AddRow();
}

void TableContralAdapter::DelRow(QModelIndexList &selecteds)
{
	DatabaseManager::GetInstance()->DelRow(selecteds);
}

void TableContralAdapter::DoubleClick(const QModelIndex &index)
{
	if (index.column() == 3){
		EditTable editTb(NULL, &index);
		editTb.exec();
	}
	else if (index.column() == 4){
		EditTable editTb(NULL, &index);
		editTb.exec();
	}
	else if (index.column() == 5){
		EditTable editTb(NULL, &index);
		editTb.exec();
	}
	else if (index.column() == 6){
		EditTable editTb(NULL, &index);
		editTb.exec();
	}
}