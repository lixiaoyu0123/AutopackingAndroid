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
	QString defaultPath = PathManager::ReadLastPath(QStringLiteral("decpack"));
	if (defaultPath.isEmpty()){
		defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	}
	QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("打开文件"), defaultPath, QStringLiteral("android packet file (*.apk)"));
	if (!fileName.isEmpty()){
		mtemplate = fileName;
		ui->LineEditPath->setText(fileName);
	}
}

void DecPackSetting::ButtonOkClickSlot()
{
	if (mtemplate.isEmpty()){
		return;
	}
	PathManager::SetDecPackPath(mtemplate);
	PathManager::WriteLastPath(QStringLiteral("decpack"), mtemplate.left(mtemplate.replace("\\", "/").lastIndexOf("/")));
}