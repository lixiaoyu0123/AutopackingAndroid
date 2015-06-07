#include "TableContralAdapterPak.h"
#include "Model/DatabaseManager.h"

void TableContralAdapterPak::AddRow()
{
	DatabaseManager::GetInstance()->AddRowPak();
}

void TableContralAdapterPak::DelRow(QModelIndexList &selecteds)
{
	DatabaseManager::GetInstance()->DelRowPak(selecteds);
}

void TableContralAdapterPak::DoubleClick(const QModelIndex &index)
{
}