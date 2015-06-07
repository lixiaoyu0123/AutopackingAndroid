#include "TableContralAdapterStr.h"
#include "Model/DatabaseManager.h"

void TableContralAdapterStr::AddRow()
{
	DatabaseManager::GetInstance()->AddRowStr();
}

void TableContralAdapterStr::DelRow(QModelIndexList &selecteds)
{
	DatabaseManager::GetInstance()->DelRowStr(selecteds);
}

void TableContralAdapterStr::DoubleClick(const QModelIndex &index)
{
}