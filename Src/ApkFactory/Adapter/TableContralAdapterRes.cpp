#include "TableContralAdapterRes.h"
#include "Model/DatabaseManager.h"


void TableContralAdapterRes::AddRow()
{
	DatabaseManager::GetInstance()->AddRowRes();
}

void TableContralAdapterRes::DelRow(QModelIndexList &selecteds)
{
	DatabaseManager::GetInstance()->DelRowRes(selecteds);
}

void TableContralAdapterRes::DoubleClick(const QModelIndex &index)
{
}