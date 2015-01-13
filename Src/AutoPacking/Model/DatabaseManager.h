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
#include "Table/ReplacePakTable.h"
#include "Table/ReplaceAppPakTable.h"

class DatabaseManager:public QObject
{
	Q_OBJECT
public:
	static DatabaseManager *GetInstance();
	static void Destroyed();
	DatabaseManager();
	~DatabaseManager();
	void InitDataModel();
	void AddRow();
	void DelRow(QModelIndexList &selecteds);
	void AddRowStr();
	void DelRowStr(QModelIndexList &selecteds);
	void AddRowRes();
	void DelRowRes(QModelIndexList &selecteds);
	void AddRowPak();
	void DelRowPak(QModelIndexList &selecteds);
	void AddRowAppPak();
	void DelRowAppPak(QModelIndexList &selecteds);
	BjTableModel *GetTableModel();
	BjTableModel *GetTableModelStr(const QModelIndex &modelIndexStr);
	BjTableModel *GetTableModelRes(const QModelIndex &modelIndexRes);
	BjTableModel *GetTableModelPak(const QModelIndex &modelIndexPak);
	BjTableModel *GetTableModelAppPak(const QModelIndex &modelIndexAppPak);
	BjTableModel *GetTableModelStr();
	BjTableModel *GetTableModelRes();
	BjTableModel *GetTableModelPak();
	BjTableModel *GetTableModelAppPak();
	int GetTableModelAppPakCounts();
	QSqlDatabase *GetDatabase();
	void Commit();
	void DeleteAll(QSqlTableModel &model);
	bool isDatabaseEmpty();
	void ChangStatInDatabase(int row, QString &status);
	void ReadyData(QString &id, QList<ReplaceStrTable> &strTable, QList<ReplaceResTable>  &resTable, QList<ReplacePakTable>  &pakTable,QList<ReplaceAppPakTable> &appPakTable);
	void ExportData(QString &fileName);
	void ImportData(QString &fileName);
	void ReloadData();

private:
	QSqlDatabase *mpdatabase;
	BjTableModel *mptableModel;
	BjTableModel *mptableModelStr;
	BjTableModel *mptableModelRes;
	BjTableModel *mptableModelPak;
	BjTableModel *mptableModelAppPak;
	QModelIndex mmodelIndexStr;
	QModelIndex mmodelIndexRes;
	QModelIndex mmodelIndexPak;
	QModelIndex mmodelIndexAppPak;
	static DatabaseManager *mpinstance;
signals:
	void ReloadDataSignal();
};
#endif