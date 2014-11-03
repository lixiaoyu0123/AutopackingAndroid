#ifndef TABLE_CONTRAL_ADAPTER_H
#define TABLE_CONTRAL_ADAPTER_H
#include <QModelIndex>
#include "Interface/ITableContral.h"

class TableContralAdapter:public ITableContral
{
public:
	TableContralAdapter();
	void AddRow();
	void DelRow(QModelIndexList &selecteds);
	void DoubleClick(const QModelIndex &index);
	void SetEnableEdit(bool isEnable);
protected:
private:
	bool misEnableEdit;
};
#endif