#ifndef CENTRAL_FRAM_H
#define CENTRAL_FRAM_H
#include <QWidget>
#include <QFrame>
#include <QTableView>
#include <QLayout>
#include <QPoint>
#include "BjTableView.h"
#include "Model/BjTableDelegate.h"
#include "Model/BjIdDelegate.h"
#include "Model/BjEditDelegate.h"

class BjCentralFram:public QWidget
{
	Q_OBJECT

public:
	explicit BjCentralFram(QWidget *parent = 0);
	~BjCentralFram();
	BjTableView &GetTableView();
protected:
	
private:
	//QFrame mfram;
	BjTableView mtableview;
	QHBoxLayout mlayout;
	BjIdDelegate *mpidDelegate;
	BjTableDelegate *mptbDelegate;
	BjEditDelegate *mpeditDelegate;

	void InitSlot();
	void InitView();
private slots:
void ReloadDataModel();
};
#endif