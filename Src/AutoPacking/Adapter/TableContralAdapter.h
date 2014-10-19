#ifndef TABLE_CONTRAL_ADAPTER_H
#define TABLE_CONTRAL_ADAPTER_H
#include <QModelIndex>
#include "Interface/ITableContral.h"

class TableContralAdapter:public ITableContral
{
public:
	void AddRow();
	void DelRow(QModelIndexList &selecteds);
	void DoubleClick(const QModelIndex &index);
protected:
private:
};
#endif