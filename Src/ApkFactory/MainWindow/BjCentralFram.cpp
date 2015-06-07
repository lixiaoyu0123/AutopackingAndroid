#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QHeaderView>
#include "BjCentralFram.h"
#include "Model/DatabaseManager.h"

BjCentralFram::BjCentralFram(QWidget *parent)
:QWidget(parent),
mtableview(parent)
{
	InitView();
	InitSlot();
}

BjCentralFram::~BjCentralFram()
{
	mpidDelegate->deleteLater();
	mptbDelegate->deleteLater();
	mpeditDelegate->deleteLater();
}

BjTableView &BjCentralFram::GetTableView()
{
	return mtableview;
}

void BjCentralFram::InitView()
{
	mlayout.addWidget(&mtableview);
	mlayout.setContentsMargins(0, 0, 0, 0);
	mlayout.setSpacing(1);
	this->setLayout(&mlayout);
	mtableview.setModel(DatabaseManager::GetInstance()->GetTableModel());
	mpidDelegate = new BjIdDelegate(this);
	mptbDelegate = new BjTableDelegate(this);
	mpeditDelegate = new BjEditDelegate(this);
	mtableview.setItemDelegateForColumn(1, mpidDelegate);
	mtableview.setItemDelegateForColumn(2, mptbDelegate);
	mtableview.setItemDelegateForColumn(3, mpeditDelegate);
	mtableview.setItemDelegateForColumn(4, mpeditDelegate);
	mtableview.setItemDelegateForColumn(5, mpeditDelegate);
	mtableview.setItemDelegateForColumn(6, mpeditDelegate);
	mtableview.setItemDelegateForColumn(7, mpeditDelegate);
	mtableview.setSortingEnabled(true);
	mtableview.hideColumn(0);
}

void BjCentralFram::InitSlot()
{
	connect(DatabaseManager::GetInstance(), SIGNAL(ReloadDataSignal()), this, SLOT(ReloadDataModel()));
}

void BjCentralFram::ReloadDataModel()
{
	mtableview.setModel(DatabaseManager::GetInstance()->GetTableModel());
}