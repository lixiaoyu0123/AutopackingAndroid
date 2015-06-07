#include "TableContralAdapterAppPak.h"
#include "Model/DatabaseManager.h"
#include "Dialogs/BjMessageBox.h"

void TableContralAdapterAppPak::AddRow()
{
	if (DatabaseManager::GetInstance()->GetTableModelAppPakCounts() >= 1){
		BjMessageBox::information(NULL, QStringLiteral("友情提示"), QStringLiteral("应用包名只有一个，所以只能替换一个"), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	DatabaseManager::GetInstance()->AddRowAppPak();
}

void TableContralAdapterAppPak::DelRow(QModelIndexList &selecteds)
{
	DatabaseManager::GetInstance()->DelRowAppPak(selecteds);
}

void TableContralAdapterAppPak::DoubleClick(const QModelIndex &index)
{
}