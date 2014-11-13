#include <QFileDialog>
#include "ZySingleSetting.h"
#include "Model/PathManager.h"

ZySingleSetting::ZySingleSetting(QWidget *parent /* = NULL */) :QDialog(parent),
ui(new Ui::ZySingleSetting)
{
	ui->setupUi(this);
	InitView();
	InitSlot();
}

ZySingleSetting::~ZySingleSetting()
{
	delete ui;
}

void ZySingleSetting::InitView()
{
	ui->LineEditChannel->setText(PathManager::GetChannelId());
	ui->LineEditBaseNm->setText(PathManager::GetBaseNm());
	ui->LineEditOriginalNm->setText(PathManager::GetOriginalNm());
	ui->LineEditResPath->setText(PathManager::GetResPath());
	ui->LineEditUrl->setText(PathManager::GetUrl());
}

void ZySingleSetting::InitSlot()
{
	connect(ui->ButtonOk, SIGNAL(clicked()), this, SLOT(ButtonOkSlot()));
	connect(ui->ButtonScan, SIGNAL(clicked()), this, SLOT(ButtonScanSlot()));
}

void ZySingleSetting::ButtonOkSlot()
{
	PathManager::SetChannelId(ui->LineEditChannel->text().trimmed());
	PathManager::SetBaseNm(ui->LineEditBaseNm->text().trimmed());
	PathManager::SetOriginalNm(ui->LineEditOriginalNm->text().trimmed());
	PathManager::SetResPath(ui->LineEditResPath->text().trimmed());
	PathManager::SetUrl(ui->LineEditUrl->text().trimmed());
	PathManager::WriteLastPath(QStringLiteral("resPath"),ui->LineEditResPath->text().trimmed());
}

void ZySingleSetting::ButtonScanSlot()
{
	QString defaultPath = PathManager::ReadLastPath(QStringLiteral("resPath"));
	if (defaultPath.isEmpty()){
		defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	}
	QString	dir = QFileDialog::getExistingDirectory(this, QStringLiteral("选择资源路径"), defaultPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()){
		ui->LineEditResPath->setText(dir);
	}
}