#ifndef EDIT_TABLE_H
#define EDIT_TABLE_H
#include <QtWidgets/QDialog>
#include <QSqlTableModel>
#include <QModelIndex>
#include "ui_EditTable.h"
#include "Adapter/TableContralAdapterStr.h"
#include "Adapter/TableContralAdapterRes.h"
#include "Adapter/TableContralAdapterPak.h"
#include "Adapter/TableContralAdapterAppPak.h"

class EditTable:public QDialog
{
	Q_OBJECT
public:
	explicit EditTable(QWidget *parent = 0, const QModelIndex *index = 0);
	~EditTable();
protected:
private:
	Ui::EditTable *ui;
	TableContralAdapterStr madapterStr;
	TableContralAdapterRes madapterRes;
	TableContralAdapterPak madapterPak;
	TableContralAdapterAppPak madapterAppPak;
	const QModelIndex *mpindex;

private:
	void InitModel(const QModelIndex *index);
	void InitView();
	void InitSlot();
private slots:
void NewItem();
void DelItem();
void OkClick();
};
#endif