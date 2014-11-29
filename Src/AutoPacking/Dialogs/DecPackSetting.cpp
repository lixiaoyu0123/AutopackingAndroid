#include <QFileDialog>
#include <QStandardPaths>
#include "DecPackSetting.h"
#include "Model/PathManager.h"

DecPackSetting::DecPackSetting(QWidget *parent) :
QDialog(parent),
ui(new Ui::DecPackSetting)
{
	ui->setupUi(this);
	InitView();
	InitSlot();
}
DecPackSetting::~DecPackSetting()
{
	delete ui;
}

void DecPackSetting::InitView()
{
	ui->LineEditPath->setText(PathManager::GetDecPackPath());
}

void DecPackSetting::InitSlot()
{
	connect(ui->ButtonScan, SIGNAL(clicked()), this, SLOT(ButtonScanClickSlot()));
	connect(ui->ButtonOk, SIGNAL(clicked()), this, SLOT(ButtonOkClickSlot()));
}

void DecPackSetting::ButtonScanClickSlot()
{
	QString defaultPath = PathManager::ReadLastPath(QStringLiteral("setBaiduSdk"));
	if (defaultPath.isEmpty()){
		defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	}
	QString	dir = QFileDialog::getExistingDirectory(this, QStringLiteral("选择百度Sdk位置"), defaultPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()){
		mtemplate = dir;
		ui->LineEditPath->setText(dir);
	}
}

void DecPackSetting::ButtonOkClickSlot()
{
	if (mtemplate.isEmpty()){
		return;
	}
	PathManager::SetDecPackPath(mtemplate);
	PathManager::WriteLastPath(QStringLiteral("setBaiduSdk"), mtemplate);
}