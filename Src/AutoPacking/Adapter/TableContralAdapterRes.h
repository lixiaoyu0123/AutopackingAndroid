#ifndef TABLE_CONTRAL_ADAPTER_RES_H
#define TABLE_CONTRAL_ADAPTER_RES_H
#include <QModelIndex>
#include "Interface/ITableContral.h"

class TableContralAdapterRes :public ITableContral
{
public:
	void AddRow();
	void DelRow(QModelIndexList &selecteds);
	void DoubleClick(const QModelIndex &index);
protected:
private:
};
#endif