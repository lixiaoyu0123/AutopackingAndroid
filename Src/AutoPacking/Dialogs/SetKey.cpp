#include <QFileDialog>
#include <QStandardPaths>
#include "SetKey.h"
#include "Model/PathManager.h"

SetKey::SetKey(QWidget *parent) :
QDialog(parent),
ui(new Ui::SetKey)
{
	ui->setupUi(this);
	InitView();
	InitSlot();	
}
SetKey::~SetKey()
{
	delete ui;
}

void SetKey::InitView()
{
	QRegExp rx("^[\\@A-Za-z0-9\\!\\#\\$\\%\\^\\&\\*\\.\\~]{6,200}$");
	QValidator *validator = new QRegExpValidator(rx, this);
	ui->LineEditKey->setValidator(validator);
	ui->LineEditPath->setText(PathManager::GetKeyPath());
	ui->LineEditKey->setText(PathManager::GetPasswd());
	ui->LineEditAliases->setText(PathManager::GetKeyAliases());
	ui->ComboBoxSigalg->insertItem(0, QString("MD5withRSA"));
	ui->ComboBoxSigalg->insertItem(1, QString("SHA1withDSA"));
	ui->ComboBoxDigestalg->insertItem(0, QString("SHA1"));
	ui->ComboBoxSigalg->setEditText(PathManager::GetSigalg());
	ui->ComboBoxDigestalg->setEditText(PathManager::GetDigestalg());
}

void SetKey::InitSlot()
{
	connect(ui->RadioButton1, SIGNAL(clicked()), this, SLOT(ChangStatusSlot()));
	connect(ui->ButtonScan, SIGNAL(clicked()), this, SLOT(ButtonScanClickSlot()));
	connect(ui->ButtonOk, SIGNAL(clicked()), this, SLOT(ButtonOkClickSlot()));
}

void SetKey::ChangStatusSlot()
{
	if (ui->RadioButton1->isChecked()){ 
		ui->LineEditKey->setEchoMode(QLineEdit::Normal);
	}
	else{ 
		ui->LineEditKey->setEchoMode(QLineEdit::Password);
	}
}

void SetKey::ButtonScanClickSlot()
{
	QString defaultPath = PathManager::ReadLastPath(QStringLiteral("setkey"));
	if (defaultPath.isEmpty()){
		defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	}
	QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("打开文件"), defaultPath, QStringLiteral("key file (*.keystore)"));
	if (!fileName.isEmpty()){
		mkeyPath = fileName;
		ui->LineEditPath->setText(fileName);
	}
}

void SetKey::ButtonOkClickSlot()
{
	PathManager::SetKeyPath(mkeyPath);
	PathManager::WriteLastPath(QStringLiteral("setkey"), mkeyPath.left(mkeyPath.replace("\\","/").lastIndexOf("/")));
	PathManager::SetPasswd(ui->LineEditKey->text());
	PathManager::SetKeyAliases(ui->LineEditAliases->text());
	PathManager::SetSigalg(ui->ComboBoxSigalg->currentText());
	PathManager::SetDigestalg(ui->ComboBoxDigestalg->currentText());
}