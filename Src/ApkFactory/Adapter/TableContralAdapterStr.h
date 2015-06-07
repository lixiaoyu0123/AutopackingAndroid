#ifndef TABLE_CONTRAL_ADAPTER_STR_H
#define TABLE_CONTRAL_ADAPTER_STR_H
#include <QModelIndex>
#include "Interface/ITableContral.h"

class TableContralAdapterStr :public ITableContral
{
public:
	void AddRow();
	void DelRow(QModelIndexList &selecteds);
	void DoubleClick(const QModelIndex &index);
protected:
private:
};
#endif