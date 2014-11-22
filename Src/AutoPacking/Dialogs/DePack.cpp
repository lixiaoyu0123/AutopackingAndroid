#include <QFileDialog>
#include <QTimer>
#include "DePack.h"
#include "Model/PathManager.h"
#include "Dialogs/BjMessageBox.h"

const int MAXPROCESS = 2;

DePack::DePack(QWidget *parent) :QDialog(parent),
ui(new Ui::DePack),
mptimer(NULL),
mpprocess(NULL)
{
	ui->setupUi(this);
	InitView();
	InitSlot();
}

DePack::~DePack()
{
	delete ui;
	if (mptimer != NULL){
		delete mptimer;
	}

	if (mpprocess != NULL){
		delete mpprocess;
	}
}

void DePack::InitView()
{
	ChangStat(false);
	ui->ProgressBarDePack->setRange(0, MAXPROCESS);
	ui->LineEditOri->setText(PathManager::ReadLastPath(QStringLiteral("depackOriPath")));
	ui->LineEditResult->setText(PathManager::ReadLastPath(QStringLiteral("depackResultPath")));
}

void DePack::InitSlot()
{
	connect(ui->ButtonScanOri, SIGNAL(clicked()), this, SLOT(ButtonScanOriSlot()));
	connect(ui->ButtonScanResult, SIGNAL(clicked()), this, SLOT(ButtonScanResultSlot()));
	connect(ui->ButtonOk, SIGNAL(clicked()), this, SLOT(ButtonOkSlot()));
	connect(ui->ButtonCancel, SIGNAL(clicked()), this, SLOT(ButtonCancelSlot()));
}

void DePack::ButtonScanOriSlot()
{
	QString defaultPath = PathManager::ReadLastPath(QStringLiteral("depackOriPath"));
	if (defaultPath.isEmpty()){
		defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	}

	QString	fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择要解包的Apk文件"), defaultPath, QStringLiteral("*.Apk"));
	if (!fileName.isEmpty()){
		ui->LineEditOri->setText(fileName);
		PathManager::WriteLastPath(QStringLiteral("depackOriPath"), fileName);
	}
}

void DePack::ButtonScanResultSlot()
{
	QString defaultPath = PathManager::ReadLastPath(QStringLiteral("depackResultPath"));
	if (defaultPath.isEmpty()){
		defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	}
	QString	dir = QFileDialog::getExistingDirectory(this, QStringLiteral("选择解包存放位置"), defaultPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()){
		ui->LineEditResult->setText(dir);
		PathManager::WriteLastPath(QStringLiteral("depackResultPath"), dir);
	}
}

void DePack::ButtonOkSlot()
{
	QFile file(ui->LineEditOri->text());
	if (ui->LineEditOri->text().isEmpty() || !file.exists()){
		BjMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("apk文件不存在或未设置apk文件位置"), QMessageBox::Ok);
		return;
	}
	QDir dir(ui->LineEditResult->text());
	if (!dir.exists()){
		dir.mkpath(ui->LineEditResult->text());
	}
	ChangStat(true);
	QString apkTool = PathManager::GetToolPath() + QStringLiteral("/apktool.bat");
	QStringList param;
	param << QString("d") << QString("-f") << ui->LineEditOri->text() << ui->LineEditResult->text();
	mpprocess = new QProcess(this);
	connect(mpprocess, SIGNAL(finished(int)), this, SLOT(FinishedSlot()));
	mpprocess->start(apkTool, param);
}

void DePack::ButtonCancelSlot()
{
	ChangStat(false);
	QProcess killer;
	killer.start("taskkill", QStringList() << "/f" << "/im" << "java.exe");
	if (!killer.waitForStarted()){
		return;
	}
	if (!killer.waitForFinished()){
		return;
	}
	if (mpprocess != NULL){
		mpprocess->terminate();
		mpprocess->deleteLater();
		mpprocess = NULL;
	}
}

void DePack::UpdateProcessSlot()
{
	int currentPos = ui->ProgressBarDePack->value();
	if (currentPos >= MAXPROCESS){
		ui->ProgressBarDePack->setValue(0);
	}
	else{
		ui->ProgressBarDePack->setValue(++currentPos);
	}
}

void DePack::FinishedSlot()
{
	ChangStat(false);
	if (mpprocess != NULL){
		mpprocess->terminate();
		mpprocess->deleteLater();
		mpprocess = NULL;
	}
	BjMessageBox::information(this, QStringLiteral("解包成功！"), QStringLiteral("恭喜，解包成功"), QMessageBox::Ok);
}

void DePack::ChangStat(bool stat)
{
	if (stat){
		ui->ButtonOk->setDisabled(true);
		ui->ButtonCancel->setEnabled(true);
		mptimer = new QTimer(this);
		connect(mptimer, SIGNAL(timeout()), this, SLOT(UpdateProcessSlot()));
		mptimer->start(200);
	}
	else{
		ui->ButtonOk->setEnabled(true);
		ui->ButtonCancel->setDisabled(true);
		if (mptimer != NULL){
			mptimer->stop();
			ui->ProgressBarDePack->setValue(0);
			delete mptimer;
			mptimer = NULL;
		}
	}
}