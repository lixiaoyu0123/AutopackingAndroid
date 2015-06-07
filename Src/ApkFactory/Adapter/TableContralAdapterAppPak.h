#ifndef TABLE_CONTRAL_ADAPTER_APPPAK_H
#define TABLE_CONTRAL_ADAPTER_APPPAK_H
#include <QModelIndex>
#include "Interface/ITableContral.h"

class TableContralAdapterAppPak :public ITableContral
{
public:
	void AddRow();
	void DelRow(QModelIndexList &selecteds);
	void DoubleClick(const QModelIndex &index);
protected:
private:
};
#endif