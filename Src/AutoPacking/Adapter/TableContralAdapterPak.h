#ifndef TABLE_CONTRAL_ADAPTER_PAK_H
#define TABLE_CONTRAL_ADAPTER_PAK_H
#include <QModelIndex>
#include "Interface/ITableContral.h"

class TableContralAdapterPak :public ITableContral
{
public:
	void AddRow();
	void DelRow(QModelIndexList &selecteds);
	void DoubleClick(const QModelIndex &index);
protected:
private:
};
#endif