#ifndef BJTABLEVIEW_H
#define BJTABLEVIEW_H
#include <QTableView>
#include <QSet>
#include <QMenu>
#include "Interface/ITableContral.h"

class BjTableView:public QTableView
{
	Q_OBJECT
public:
	explicit BjTableView(QWidget *parent = 0);
	~BjTableView();
	void SetAapter(ITableContral *pinterface);
	void AddAction(QAction *action);
	QModelIndexList GetSelectIndexs();
protected:
	void contextMenuEvent(QContextMenuEvent *event);
private:
	QMenu mmenu;
	QAction *pactionNew;
	QAction *pactionDel;
	QSet<QAction *> mactionSet;
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