#ifndef ITABLE_CONTRAL_H
#define ITABLE_CONTRAL_H
#include <QModelIndex>

class ITableContral
{
public:
	virtual void AddRow() = 0;
	virtual void DelRow(QModelIndexList &selecteds) = 0;
	virtual void DoubleClick(const QModelIndex &index) = 0;
protected:
private:
};
#endif