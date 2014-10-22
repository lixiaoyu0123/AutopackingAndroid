#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QProcess>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Model/DatabaseManager.h"
#include "Model/PathManager.h"
#include "Dialogs/About.h"
#include "Dialogs/BjMessageBox.h"
#include "Dialogs/ImportData.h"
#include "Dialogs/ExportData.h"
#include "Dialogs/SetJdkPath.h"
#include "Dialogs/SetKey.h"
#include "Dialogs/SourcePackSetting.h"
#include "Dialogs/DecPackSetting.h"
#include "Dialogs/ThreadConfigDialog.h"
#include "Dialogs/SetVersionDialog.h"
#include "Works/DecPack.h"
#include "Works/SrcPack.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	mtoolBar(parent),
	mcentralFram(parent),
	mstatusBar(parent),
	mthreadNum(PathManager::GetThreadNum()),
	mversion(PathManager::GetVersion()),
	mcurrentTaskIndex(0)
{
    ui->setupUi(this);
	InitView();
	InitData();
	InitSlot();
}

MainWindow::~MainWindow()
{
    delete ui;
	DatabaseManager::GetInstance()->Destroyed();
}

void MainWindow::InitData()
{
	mcentralFram.GetTableView().SetAapter(&madapter);
	StatusTextChang();
}

void MainWindow::InitView()
{
	this->addToolBar((QToolBar *)&mtoolBar);
	this->setCentralWidget(&mcentralFram);
	this->setStatusBar(&mstatusBar);
}

void MainWindow::InitSlot()
{
	connect(ui->actionVersion, SIGNAL(triggered()), this, SLOT(VersionSlot()));
	connect(ui->actionImportDb, SIGNAL(triggered()), this, SLOT(ImportDataSlot()));
	connect(ui->actionExportDb, SIGNAL(triggered()), this, SLOT(ExportDataSlot()));
	connect(ui->actionSetJdk, SIGNAL(triggered()), this, SLOT(SetJdkPathSlot()));
	connect(ui->actionSetKey, SIGNAL(triggered()), this, SLOT(SetKeySlot()));
	connect(ui->actionSourceSetting, SIGNAL(triggered()), this, SLOT(SourcePackSettingSlot()));
	connect(ui->actionDecPkSetting, SIGNAL(triggered()), this, SLOT(DecPackSettingSlot()));
	connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(HeldSlot()));
	connect(ui->actionUpdata, SIGNAL(triggered()), this, SLOT(UpdataSlot()));
	connect(ui->actionSetVersion, SIGNAL(triggered()), this, SLOT(SetVersionSlot()));
	connect(mtoolBar.GetActionStar(), SIGNAL(triggered()), this, SLOT(StartSlot()));
	connect(mtoolBar.GetActionStop(), SIGNAL(triggered()), this, SLOT(StopSlot()));
	connect(mtoolBar.GetButtonLog(), SIGNAL(clicked()), this, SLOT(ShowLogSlot()));
	connect(mtoolBar.GetButtonThreadConfig(), SIGNAL(clicked()), this, SLOT(ThreadConfigSlot()));
}

void MainWindow::ChangStat(bool isStar)
{
	mtoolBar.GetActionStar()->setDisabled(isStar);
	mtoolBar.GetActionStop()->setEnabled(isStar);
	QDateTime currentDateTime = QDateTime::currentDateTime();
	QString currentDate = currentDateTime.toString("yyyy-MM-dd hh:mm:ss ddd");
	if (isStar){ 
		mstartTime = currentDateTime;
		mstatusBar.ShowTime(QStringLiteral("打包开始： ") + currentDate + QStringLiteral("  "));
		mlog.clear();
		mcentralFram.GetTableView().setDisabled(true);
		mtoolBar.GetButtonThreadConfig()->setDisabled(true);
		mtoolBar.GetCombox()->setDisabled(true);
		mtoolBar.GetButtonScan()->setDisabled(true);
		ui->menuBar->setDisabled(true);
	}
	else{
		mstatusBar.ShowTime(QStringLiteral("打包结束： ") + currentDate + QStringLiteral("  "));
		mcentralFram.GetTableView().setEnabled(true);
		mtoolBar.GetButtonThreadConfig()->setEnabled(true);
		mtoolBar.GetCombox()->setEnabled(true);
		mtoolBar.GetButtonScan()->setEnabled(true);
		ui->menuBar->setEnabled(true);
	}
}

void MainWindow::StartSlot()
{
	ChangStat(true);
	if (DatabaseManager::GetInstance()->isDatabaseEmpty()){
		BjMessageBox::warning(NULL, QStringLiteral("数据为空"), QStringLiteral("没有配置数据，请先配置数据表！"), QMessageBox::Ok, QMessageBox::NoButton);
		ChangStat(false);
		return;
	}

	QModelIndexList selectedIndex = mcentralFram.GetTableView().GetSelectIndexs();
	int rows = DatabaseManager::GetInstance()->GetTableModel()->rowCount();
	if (!selectedIndex.isEmpty()){
		mrecordIndex = QVector<int>(selectedIndex.size());
		int i = 0;
		for (QModelIndexList::iterator ite = selectedIndex.begin(); ite != selectedIndex.end(); ite++)
		{
			mrecordIndex[i++] = ite->row();
		}
	}
	else{
		mrecordIndex = QVector<int>(rows);
		for (int i = 0; i < rows; i++)
		{
			mrecordIndex[i] = i;
		}
	}

	mcurrentTaskIndex = 0;
	switch (mtoolBar.GetCombox()->currentIndex())
	{
	case 0:
		StartSrcPack();
		break;
	case 1:
		StartDecPack();
		break;
	}
}

void MainWindow::StartDecPack()
{
	for (int i = 0; i < mthreadNum; i++,mcurrentTaskIndex++)
	{
		if (mcurrentTaskIndex >= mrecordIndex.size()){
			break;;
		}
		Pack *ppack = new DecPack(this);
		mtaskList.push_back(ppack);
		connect(ppack, SIGNAL(FinishSignal(int,int)), this, SLOT(FinishedSlot(int,int)), Qt::QueuedConnection);
		connect(ppack, SIGNAL(GenerateError(QString)), SLOT(CollectLog(QString)), Qt::QueuedConnection);
		QString id = DatabaseManager::GetInstance()->GetTableModel()->record(mrecordIndex.at(mcurrentTaskIndex)).value("ID").toString();
		QString channelId = DatabaseManager::GetInstance()->GetTableModel()->record(mrecordIndex.at(mcurrentTaskIndex)).value("ChannelID").toString();
		QString channelName = DatabaseManager::GetInstance()->GetTableModel()->record(mrecordIndex.at(mcurrentTaskIndex)).value("ChannelName").toString();
		QList<ReplaceStrTable> strTableList;
		QList<ReplaceResTable> resTableList;
		QList<ReplacePakTable> pakTableList;
		DatabaseManager::GetInstance()->ChangStatInDatabase(mrecordIndex.at(mcurrentTaskIndex), QStringLiteral("打包开始！"));
		DatabaseManager::GetInstance()->ReadyData(id, strTableList, resTableList, pakTableList);
		ppack->Start(PathManager::GetDecPackPath(), PathManager::GetOutPath(), channelId, channelName, id, strTableList, resTableList, pakTableList, mcurrentTaskIndex);
	}

	if (mtaskList.isEmpty()){
		ChangStat(false);
	}
}

void MainWindow::StartSrcPack()
{
	for (int i = mtaskList.size(); i < mthreadNum; i++, mcurrentTaskIndex++)
	{
		if (mcurrentTaskIndex >= mrecordIndex.size()){
			break;;
		}
		Pack *ppack = new SrcPack(this);
		mtaskList.push_back(ppack);
		connect(ppack, SIGNAL(FinishSignal(int, int)), this, SLOT(FinishedSlot(int, int)), Qt::QueuedConnection);
		connect(ppack, SIGNAL(GenerateError(QString)), SLOT(CollectLog(QString)), Qt::QueuedConnection);
		QString id = DatabaseManager::GetInstance()->GetTableModel()->record(mrecordIndex.at(mcurrentTaskIndex)).value("ID").toString();
		QString channelId = DatabaseManager::GetInstance()->GetTableModel()->record(mrecordIndex.at(mcurrentTaskIndex)).value("ChannelID").toString();
		QString channelName = DatabaseManager::GetInstance()->GetTableModel()->record(mrecordIndex.at(mcurrentTaskIndex)).value("ChannelName").toString();
		QList<ReplaceStrTable> strTableList;
		QList<ReplaceResTable> resTableList;
		QList<ReplacePakTable> pakTableList;
		DatabaseManager::GetInstance()->ChangStatInDatabase(mrecordIndex.at(mcurrentTaskIndex), QStringLiteral("打包开始！"));
		DatabaseManager::GetInstance()->ReadyData(id, strTableList, resTableList, pakTableList);
		ppack->Start(PathManager::GetDecPackPath(), PathManager::GetOutPath(), channelId, channelName, id, strTableList, resTableList, pakTableList, mcurrentTaskIndex);
	}

	if (mtaskList.isEmpty()){
		ChangStat(false);
	}
}

void MainWindow::FinishedSlot(int stat,int taskId)
{
	switch (stat)
	{
	case 0:
		for each (Pack * var in mtaskList)
		{
			if (var->GetTaskId() == taskId){
				var->deleteLater();
				mtaskList.removeOne(var);
				DatabaseManager::GetInstance()->ChangStatInDatabase(taskId, QStringLiteral("打包完成!"));
			}
		}
		break;
	case 1:
		for each (Pack * var in mtaskList)
		{
			if (var->GetTaskId() == taskId){
				var->deleteLater();
				mtaskList.removeOne(var);
				DatabaseManager::GetInstance()->ChangStatInDatabase(taskId, QStringLiteral("打包出错!"));
			}
		}
		break;
	case 2:
		for each (Pack * var in mtaskList)
		{
			if (var->GetTaskId() == taskId){
				var->deleteLater();
				mtaskList.removeOne(var);
				DatabaseManager::GetInstance()->ChangStatInDatabase(taskId, QStringLiteral("打包停止!"));
			}
		}
		return;
	}

	if (mtaskList.size() < mthreadNum){
		switch (mtoolBar.GetCombox()->currentIndex())
		{
		case 0:
			StartSrcPack();
			break;
		case 1:
			StartDecPack();
			break;
		}
		
	}
}

void MainWindow::StopSlot()
{
	ChangStat(false);
	for each (Pack *var in mtaskList)
	{
		var->Stop();
	}
}

void MainWindow::VersionSlot()
{
	About ab(this);
	ab.exec();
}

void MainWindow::ImportDataSlot()
{
	ImportData imData(this);
	imData.exec();
}

void MainWindow::ExportDataSlot()
{
	ExportData exData(this);
	exData.exec();
}

void MainWindow::SetJdkPathSlot()
{
	SetJdkPath setJdk(this);
	setJdk.exec();
}

void MainWindow::SetKeySlot()
{
	SetKey setK(this);
	setK.exec();
}

void MainWindow::SourcePackSettingSlot()
{
	SourcePackSetting srcPackSetting(this);
	srcPackSetting.exec();
}

void MainWindow::DecPackSettingSlot()
{
	DecPackSetting decPackSet(this);
	decPackSet.exec();
}

void MainWindow::SetVersionSlot()
{
	SetVersionDialog setVerDialog(this);
	if (setVerDialog.exec() == QDialog::Accepted){
		mversion = PathManager::GetVersion();
		StatusTextChang();
	}
}

void MainWindow::ShowLogSlot()
{
	LogDialog logDia(this);
	logDia.SetText(mlog);
	logDia.exec();
}

void MainWindow::HeldSlot()
{
	QString arg = PathManager::GetStartPath() + "/UserManual.chm";
	QProcess::startDetached("hh.exe",QStringList()<<arg);
}

void MainWindow::UpdataSlot()
{
	BjMessageBox::warning(this, QStringLiteral("检查更新"), QStringLiteral("暂无更新！"), QMessageBox::Ok, QMessageBox::NoButton);
}

void MainWindow::ThreadConfigSlot()
{
	ThreadConfigDialog threadConfig(this);
	if (threadConfig.exec() == QDialog::Accepted){		
		mthreadNum = PathManager::GetThreadNum();
		StatusTextChang();
	}
}

void MainWindow::CollectLog(QString log)
{
	mlog.append(log);
}

void MainWindow::StatusTextChang()
{
	mstatusBar.SetText1(QStringLiteral("线程数量：%1  当前打包版本:%2").arg(QString::number(mthreadNum)).arg(mversion));
}