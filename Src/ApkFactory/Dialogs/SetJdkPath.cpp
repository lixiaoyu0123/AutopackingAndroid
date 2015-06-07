#include <QFileDialog>
#include <QStandardPaths>
#include "SetJdkPath.h"
#include "Model/PathManager.h"


SetJdkPath::SetJdkPath(QWidget *parent) :
QDialog(parent),
ui(new Ui::SetJdkPath)
{
	ui->setupUi(this);
	InitSlot();
	InitView();
}
SetJdkPath::~SetJdkPath()
{
	delete ui;
}

void SetJdkPath::InitSlot()
{
	connect(ui->ButtonScan, SIGNAL(clicked()), this, SLOT(ButtonScanClickSlot()));
	connect(ui->ButtonOk, SIGNAL(clicked()), this, SLOT(ButtonOkClickSlot()));
}

void SetJdkPath::InitView()
{
	ui->LineEditPath->setText(PathManager::GetJdkPath());
}

void SetJdkPath::ButtonScanClickSlot()
{
	QString defaultPath = PathManager::ReadLastPath(QStringLiteral("setjdk"));
	if (defaultPath.isEmpty()){
		defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	}
	QString	dir = QFileDialog::getExistingDirectory(this, QStringLiteral("Ñ¡ÔñjdkÂ·¾¶"), defaultPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()){
		mjdkPath = dir;
		ui->LineEditPath->setText(dir);
	}
}

void SetJdkPath::ButtonOkClickSlot()
{
	PathManager::SetJdkPath(mjdkPath);
	PathManager::WriteLastPath(QStringLiteral("setjdk"), mjdkPath);
}