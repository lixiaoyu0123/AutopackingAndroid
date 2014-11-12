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
}

void ZySingleSetting::ButtonOkSlot()
{
	PathManager::SetChannelId(ui->LineEditChannel->text().trimmed());
	PathManager::SetBaseNm(ui->LineEditBaseNm->text().trimmed());
	PathManager::SetOriginalNm(ui->LineEditOriginalNm->text().trimmed());
	PathManager::SetResPath(ui->LineEditResPath->text().trimmed());
	PathManager::SetUrl(ui->LineEditUrl->text().trimmed());
}