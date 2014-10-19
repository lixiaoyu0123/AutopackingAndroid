#ifndef BJTABLEVIEW_H
#define BJTABLEVIEW_H
#include <QTableView>
#include <QMenu>
#include "Interface/ITableContral.h"

class BjTableView:public QTableView
{
	Q_OBJECT
public:
	explicit BjTableView(QWidget *parent = 0);
	~BjTableView();
	void SetAapter(ITableContral *pinterface);
	QModelIndexList GetSelectIndex();
protected:
	void contextMenuEvent(QContextMenuEvent *event);
private:
	QMenu mmenu;
	QAction *pactionNew;
	QAction *pactionDel;
	ITableContral *mpinterface;

private:
	void InitSlot();
	void InitView();
private slots:
	void ActionNewSlot();
	void ActionDelSlot();
	void DoubleClick(const QModelIndex &index);
};
#endif