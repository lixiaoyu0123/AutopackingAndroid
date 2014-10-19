#include <QFile>
#include <QStandardItemModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>
#include "DatabaseManager.h"
#include "PathManager.h"
#include "Dialogs/BjMessageBox.h"

DatabaseManager* DatabaseManager::mpinstance = NULL;

DatabaseManager::DatabaseManager() :
QObject(),
mptableModel(NULL),
mptableModelStr(NULL),
mptableModelRes(NULL),
mmodelIndexStr(),
mmodelIndexRes()
{
	InitDataModel();
}

DatabaseManager::~DatabaseManager()
{
	Commit();
	mdatabase.close();
	if (mptableModel != NULL){
		mptableModel->deleteLater();
		mptableModel = NULL;
	}
	if (mptableModelStr != NULL){
		mptableModelStr->deleteLater();
		mptableModelStr = NULL;
	}
	if (mptableModelRes != NULL){
		mptableModelRes->deleteLater();
		mptableModelRes = NULL;
	}
}

DatabaseManager *DatabaseManager::GetInstance()
{
	if (mpinstance == NULL){
		mpinstance = new DatabaseManager;
	}
	return mpinstance;
}

void DatabaseManager::Destroyed()
{
	if (mpinstance != NULL){
		delete mpinstance;
		mpinstance = NULL;
	}
}

void DatabaseManager::InitDataModel()
{
	mdatabase = QSqlDatabase::addDatabase("QSQLITE");
	mdatabase.setDatabaseName(PathManager::GetDataBasePath());
	if (!QFile::exists(PathManager::GetDataBasePath())){
		if (!mdatabase.open()){
			BjMessageBox msg;
			msg.setText(QStringLiteral("数据库创建失败！"));
			msg.exec();
			exit(1);
		};
		QSqlQuery query;
		bool isSuccess = query.exec("CREATE TABLE Channeltb ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
			"ChannelID INTEGER, "
			"ChannelName VARCHAR(256), "
			"ReplaceString VARCHAR(256), "
			"ReplaceRes VARCHAR(256), "
			"PackStatus VARCHAR(80))");

		if (!isSuccess){
			BjMessageBox msg;
			msg.setText(QStringLiteral("创建Channeltb失败！"));
			msg.exec();
			mdatabase.close();
			QFile::remove(PathManager::GetDataBasePath());
			exit(1);
		}

		isSuccess = query.exec("CREATE TABLE Stringtb ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
			"ChanneltbID INTEGER, "
			"Path VARCHAR(256), "
			"SourceString VARCHAR(256), "
			"TargetString VARCHAR(256)) ");
		if (!isSuccess){
			BjMessageBox msg;
			msg.setText(QStringLiteral("创建Stringtb失败！"));
			msg.exec();
			mdatabase.close();
			QFile::remove(PathManager::GetDataBasePath());
			exit(1);
			return;
		}

		isSuccess = query.exec("CREATE TABLE Resourcetb ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
			"ChanneltbID INTEGER, "
			"SourceResDir VARCHAR(256), "
			"TargetResDir VARCHAR(256)) ");
		if (!isSuccess){
			BjMessageBox msg;
			msg.setText(QStringLiteral("创建Resourcetb失败！"));
			msg.exec();
			mdatabase.close();
			QFile::remove(PathManager::GetDataBasePath());
			exit(1);
			return;
		}
	}
	else{
		if (!mdatabase.open()){
			BjMessageBox msg;
			msg.setText(QStringLiteral("数据库打开失败！"));
			msg.exec();
			QFile::remove(PathManager::GetDataBasePath());
			exit(1);
		}
	}

	mptableModel = new BjTableModel(NULL, mdatabase);
	mptableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	mptableModel->setTable("Channeltb");
	if (!mptableModel->select()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModel->lastError().text()), QMessageBox::Ok, QMessageBox::NoButton);
		mptableModel->revertAll();//如果不删除，则撤销
		return;
	}
	mptableModel->setHeaderData(1, Qt::Horizontal, QStringLiteral("渠道ID"));
	mptableModel->setHeaderData(2, Qt::Horizontal, QStringLiteral("渠道名称"));
	mptableModel->setHeaderData(3, Qt::Horizontal, QStringLiteral("编辑替换字符串"));
	mptableModel->setHeaderData(4, Qt::Horizontal, QStringLiteral("编辑替换资源"));
	mptableModel->setHeaderData(5, Qt::Horizontal, QStringLiteral("状态"));


	mptableModelStr = new BjTableModel(NULL, mdatabase);
	mptableModelStr->setEditStrategy(QSqlTableModel::OnManualSubmit);
	mptableModelStr->setTable("Stringtb");
	if (!mptableModelStr->select()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelStr->lastError().text()), QMessageBox::Ok, QMessageBox::NoButton);
		mptableModelStr->revertAll();//如果不删除，则撤销
		return;
	}
	mptableModelStr->setHeaderData(2, Qt::Horizontal, QStringLiteral("文件相对路径"));
	mptableModelStr->setHeaderData(3, Qt::Horizontal, QStringLiteral("原节点"));
	mptableModelStr->setHeaderData(4, Qt::Horizontal, QStringLiteral("替换后节点"));

	mptableModelRes = new BjTableModel(NULL, mdatabase);
	mptableModelRes->setEditStrategy(QSqlTableModel::OnManualSubmit);
	mptableModelRes->setTable("Resourcetb");
	if (!mptableModelRes->select()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelRes->lastError().text()), QMessageBox::Ok, QMessageBox::NoButton);
		mptableModelRes->revertAll();//如果不删除，则撤销
		return;
	}
	mptableModelRes->setHeaderData(2, Qt::Horizontal, QStringLiteral("原资源所在目录"));
	mptableModelRes->setHeaderData(3, Qt::Horizontal, QStringLiteral("替换资源所在目录"));
}

void DatabaseManager::AddRow()
{
	int rowNum = mptableModel->rowCount();//获得表的行数
	QSqlRecord record = mptableModel->record();
	QSqlField fieldStr("ReplaceString", QVariant::Char);
	QSqlField fieldRes("ReplaceRes", QVariant::Char);
	fieldStr.setValue(QStringLiteral("双击编辑"));
	fieldRes.setValue(QStringLiteral("双击编辑"));
	record.append(fieldStr);
	record.append(fieldRes);

	if (!mptableModel->insertRecord(rowNum, record)){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModel->lastError().text()), QMessageBox::Ok, QMessageBox::NoButton);
		mptableModel->revertAll();//如果不删除，则撤销
		return;
	}

	if (!mptableModel->submitAll()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModel->lastError().text()));
		mptableModel->revertAll();//如果不删除，则撤销
	}
}

void DatabaseManager::DelRow(QModelIndexList &selecteds)
{
	foreach(QModelIndex index, selecteds)
	{
		int curRow = index.row(); //删除所有被选中的行		
		if (!mptableModel->removeRow(curRow)){
			BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModel->lastError().text()));
			mptableModel->revertAll();//如果不删除，则撤销
			return;
		}
		QString id = mptableModel->record(curRow).value("ID").toString();
		mptableModelStr->setFilter(QString("ChanneltbID=\'%1\'").arg(id));
		if (!mptableModelStr->select()){
			BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelStr->lastError().text()));
			mptableModelStr->revertAll();//如果不删除，则撤销
		}
		DeleteAll(*mptableModelStr);

		mptableModelRes->setFilter(QString("ChanneltbID=\'%1\'").arg(id));
		if (!mptableModelRes->select()){
			BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelRes->lastError().text()));
			mptableModelRes->revertAll();//如果不删除，则撤销
		}
		DeleteAll(*mptableModelRes);
	}

	if (!mptableModel->submitAll()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelStr->lastError().text()));
		mptableModel->revertAll();//如果不删除，则撤销
	}
}

void DatabaseManager::AddRowStr()
{
	int rowNum = mptableModelStr->rowCount();//获得表的行数
	QString id = mptableModel->record(mmodelIndexStr.row()).value("ID").toString();
	QSqlRecord record = mptableModelStr->record();
	QSqlField fieldChannltbId("ChanneltbID", QVariant::Int);
	fieldChannltbId.setValue(id);
	record.append(fieldChannltbId);

	if (!mptableModelStr->insertRecord(rowNum, record)){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelStr->lastError().text()), QMessageBox::Ok, QMessageBox::NoButton);
		mptableModelStr->revertAll();//如果不删除，则撤销
		return;
	}

	if (!mptableModelStr->submitAll()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelStr->lastError().text()));
		mptableModelStr->revertAll();//如果不删除，则撤销
	}

}

void DatabaseManager::DelRowStr(QModelIndexList &selecteds)
{
	foreach(QModelIndex index, selecteds)
	{
		int curRow = index.row(); //删除所有被选中的行		
		if (!mptableModelStr->removeRow(curRow)){
			BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelStr->lastError().text()));
			mptableModelStr->revertAll();//如果不删除，则撤销
			return;
		}
	}

	if (!mptableModelStr->submitAll()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelStr->lastError().text()));
		mptableModelStr->revertAll();//如果不删除，则撤销
	}
}

void DatabaseManager::AddRowRes()
{
	int rowNum = mptableModelRes->rowCount();//获得表的行数
	QString id = mptableModel->record(mmodelIndexRes.row()).value("ID").toString();
	QSqlRecord record = mptableModelRes->record();
	QSqlField fieldChannltbId("ChanneltbID", QVariant::Int);
	fieldChannltbId.setValue(id);
	record.append(fieldChannltbId);


	if (!mptableModelRes->insertRecord(rowNum, record)){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelRes->lastError().text()), QMessageBox::Ok, QMessageBox::NoButton);
		mptableModelRes->revertAll();//如果不删除，则撤销
		return;
	}

	if (!mptableModelRes->submitAll()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelRes->lastError().text()));
		mptableModelRes->revertAll();//如果不删除，则撤销
	}
}

void DatabaseManager::DelRowRes(QModelIndexList &selecteds)
{
	foreach(QModelIndex index, selecteds)
	{
		int curRow = index.row(); //删除所有被选中的行		
		if (!mptableModelRes->removeRow(curRow)){
			BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelRes->lastError().text()));
			mptableModelRes->revertAll();//如果不删除，则撤销
			return;
		}
	}

	if (!mptableModelRes->submitAll()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelRes->lastError().text()));
		mptableModelRes->revertAll();//如果不删除，则撤销
	}
}

void DatabaseManager::DeleteAll(QSqlTableModel &model)
{
	int rowNum = model.rowCount();
	if (rowNum == 0){
		return;
	}

	if (!model.removeRows(0, rowNum)){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(model.lastError().text()));
		model.revertAll();//如果不删除，则撤销
	 }

	if (!model.submitAll()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(model.lastError().text()));
		model.revertAll();//如果不删除，则撤销
	}
}

bool DatabaseManager::isDatabaseEmpty()
{
	if (mptableModel->rowCount() == 0){
		return true;
	}
	return false;
}

BjTableModel *DatabaseManager::GetTableModel()
{
	return mptableModel;
}

QSqlDatabase *DatabaseManager::GetDatabase()
{
	return &mdatabase;
}

BjTableModel *DatabaseManager::GetTableModelStr(const QModelIndex &modelIndexStr)
{
	mmodelIndexStr = modelIndexStr;
	QString id = mptableModel->record(modelIndexStr.row()).value("ID").toString();
	mptableModelStr->setFilter(QString("ChanneltbID=\'%1\'").arg(id));
	if (!mptableModelStr->select()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelRes->lastError().text()), QMessageBox::Ok, QMessageBox::NoButton);
		mptableModel->revertAll();//如果不删除，则撤销
	}
	return mptableModelStr;
}

BjTableModel *DatabaseManager::GetTableModelRes(const QModelIndex &modelIndexRes)
{
	mmodelIndexRes = modelIndexRes;
	QString id = mptableModel->record(mmodelIndexRes.row()).value("ID").toString();
	mptableModelRes->setFilter(QString("ChanneltbID=\'%1\'").arg(id));
	if (!mptableModelRes->select()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModelRes->lastError().text()), QMessageBox::Ok, QMessageBox::NoButton);
		mptableModel->revertAll();//如果不删除，则撤销
	}
	return mptableModelRes;
}

BjTableModel *DatabaseManager::GetTableModelStr()
{
	return mptableModelStr;
}

BjTableModel *DatabaseManager::GetTableModelRes()
{
	return mptableModelRes;
}

void DatabaseManager::ChangStatInDatabase(int row, QString &status)
{
	QSqlRecord record = mptableModel->record(row);
	record.setValue("PackStatus", status);
	if (!mptableModel->setRecord(row, record)){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModel->lastError().text()));
		mptableModel->revertAll();//如果不删除，则撤销
	}
	if (!mptableModel->submitAll()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1").arg(mptableModel->lastError().text()));
		mptableModel->revertAll();//如果不删除，则撤销
	}
}

void DatabaseManager::ReadyData(QString &id, QList<ReplaceStrTable> &strTable, QList<ReplaceResTable>  &resTable)
{
	mptableModelStr->setFilter(QString("ChanneltbID=\'%1\'").arg(id));
	int rowNum = mptableModelStr->rowCount();
	for (int i = 0; i < rowNum; i++)
	{
		QString path = mptableModelStr->record(i).value("Path").toString().trimmed();
		QString srcStr = mptableModelStr->record(i).value("SourceString").toString();
		QString destStr = mptableModelStr->record(i).value("TargetString").toString();
		ReplaceStrTable reStrTable;
		reStrTable.SetFolder(path);
		reStrTable.SetSrcStr(srcStr);
		reStrTable.SetDestStr(destStr);
		strTable.push_back(reStrTable);
	}

	mptableModelRes->setFilter(QString("ChanneltbID=\'%1\'").arg(id));
	rowNum = mptableModelRes->rowCount();
	for (int i = 0; i < rowNum; i++)
	{
		QString srcDir = mptableModelRes->record(i).value("SourceResDir").toString().trimmed();
		QString destDir = mptableModelRes->record(i).value("TargetResDir").toString().trimmed();
		ReplaceResTable reResTable;
		reResTable.SetFolderSrc(srcDir);
		reResTable.SetFolderDest(destDir);
		resTable.push_back(reResTable);
	}
}

void DatabaseManager::ExportData(QString &fileName)
{
	Commit();
	mdatabase.close();
	if (!PathManager::CopyFile(PathManager::GetDataBasePath(), fileName, true)){
		BjMessageBox::warning(NULL, QStringLiteral("导出数据出错！"), QStringLiteral("导出数据错误!"), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	BjMessageBox::information(NULL, QStringLiteral("导出成功！"), QStringLiteral("导出成功！"), QMessageBox::Ok, QMessageBox::NoButton);
	ReloadData();
}

void DatabaseManager::ImportData(QString &fileName)
{
	Commit();
	mdatabase.close();
	if (!PathManager::CopyFile(fileName,PathManager::GetDataBasePath(), true)){
		BjMessageBox::warning(NULL, QStringLiteral("导入数据出错！"), QStringLiteral("导入数据错误!"), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	BjMessageBox::information(NULL, QStringLiteral("导入成功！"), QStringLiteral("导入成功！"), QMessageBox::Ok, QMessageBox::NoButton);
	ReloadData();
}

void DatabaseManager::ReloadData()
{
	mdatabase.close();
	if (mptableModel != NULL){
		mptableModel->deleteLater();
		mptableModel = NULL;
	}
	if (mptableModelStr != NULL){
		mptableModelStr->deleteLater();
		mptableModelStr = NULL;
	}
	if (mptableModelRes != NULL){
		mptableModelRes->deleteLater();
		mptableModelRes = NULL;
	}
	if (!mdatabase.open()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库打开出错！"), QStringLiteral("数据库打开失败!"), QMessageBox::Ok, QMessageBox::NoButton);
	}
	InitDataModel();
	emit ReloadDataSignal();
}

void DatabaseManager::Commit()
{
	if (!mdatabase.transaction()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("开始事务处理出错!"), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	if (!mptableModel->submitAll() || !mptableModelStr->submitAll() || !mptableModelRes->submitAll()){
		BjMessageBox::warning(NULL, QStringLiteral("数据库错误"), QStringLiteral("数据库错误: %1 %2 %3")
			.arg(mptableModel->lastError().text())
			.arg(mptableModelStr->lastError().text())
			.arg(mptableModelRes->lastError().text()));
		mdatabase.rollback();//回滚
		return;
	}
	mdatabase.commit();//提交
}