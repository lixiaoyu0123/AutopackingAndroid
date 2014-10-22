#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QTableView>
#include <QDateTime>
#include "BjToolBar.h"
#include "BjStatusBar.h"
#include "BjCentralFram.h"
#include "Dialogs/LogDialog.h"
#include "Interface/ITableContral.h"
#include "Adapter/TableContralAdapter.h"
#include "Works/Pack.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void InitData();
	void InitView();
	void InitSlot();
	void ChangStat(bool isStar);
	void StatusTextChang();
	void StartDecPack(QVector<int> &recorderRows);
	void StartSrcPack(QVector<int> &recorderRows);

public slots:
	void StartSlot();
	void StopSlot();
	void VersionSlot();
	void ImportDataSlot();
	void ExportDataSlot();
	void SetJdkPathSlot();
	void SetKeySlot();
	void SourcePackSettingSlot();
	void DecPackSettingSlot();
	void FinishedSlot(int stat,int taskId);
	void ShowLogSlot();
	void CollectLog(QString log);
	void HeldSlot();
	void ThreadConfigSlot();
	void UpdataSlot();
	void SetVersionSlot();

private:
    Ui::MainWindow *ui;
	BjToolBar mtoolBar;
	BjCentralFram mcentralFram;
	BjStatusBar mstatusBar;
	QDateTime mstartTime;
	TableContralAdapter madapter;
	QString mlog;
	int mthreadNum;
	int mcurrentTaskIndex;
	int mrowNum;
	QList<Pack *> mtaskList;
	QString mversion;
};

#endif // MAINWINDOW_H
