#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H
#include <QSqlDatabase> 
#include <QSqlQuery> 
#include <QObject> 
#include <QString>
#include <QTableView>
#include <QModelIndex>
#include "Model/BjTableDelegate.h"
#include "Model/BjTableModel.h"
#include "Table/ReplaceStrTable.h"
#include "Table/ReplaceResTable.h"

class DatabaseManager:public QObject
{
	Q_OBJECT
public:
	static DatabaseManager *GetInstance();
	static void Destroyed();
	DatabaseManager::DatabaseManager();
	DatabaseManager::~DatabaseManager();
	void InitDataModel();
	void AddRow();
	void DelRow(QModelIndexList &selecteds);
	void AddRowStr();
	void DelRowStr(QModelIndexList &selecteds);
	void AddRowRes();
	void DelRowRes(QModelIndexList &selecteds);
	BjTableModel *GetTableModel();
	BjTableModel *GetTableModelStr(const QModelIndex &modelIndexStr);
	BjTableModel *GetTableModelRes(const QModelIndex &modelIndexRes);
	BjTableModel *GetTableModelStr();
	BjTableModel *GetTableModelRes();
	QSqlDatabase *GetDatabase();
	void Commit();
	void DeleteAll(QSqlTableModel &model);
	bool isDatabaseEmpty();
	void ChangStatInDatabase(int row, QString &status);
	void DatabaseManager::ReadyData(QString &id, QList<ReplaceStrTable> &strTable, QList<ReplaceResTable>  &resTable);
	void ExportData(QString &fileName);
	void ImportData(QString &fileName);
	void ReloadData();

private:
	QSqlDatabase mdatabase;
	BjTableModel *mptableModel;
	BjTableModel *mptableModelStr;
	BjTableModel *mptableModelRes;
	QModelIndex mmodelIndexStr;
	QModelIndex mmodelIndexRes;
	static DatabaseManager *mpinstance;
signals:
	void ReloadDataSignal();
};
#endif