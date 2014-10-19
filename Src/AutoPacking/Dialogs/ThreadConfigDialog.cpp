#include <QRegExpValidator>
#include "ThreadConfigDialog.h"
#include "Model/PathManager.h"

ThreadConfigDialog::ThreadConfigDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::ThreadConfigDialog)
{
	ui->setupUi(this);
	InitView();
	InitSlot();
}

ThreadConfigDialog::~ThreadConfigDialog()
{
	delete ui;
}

void ThreadConfigDialog::InitView()
{
	QRegExp regExp("[1]{0,1}[0-9]{0,1}");
	ui->LineEditThread->setValidator(new QRegExpValidator(regExp, this));
	ui->LineEditThread->setText(QString::number(PathManager::GetThreadNum()));
}

void ThreadConfigDialog::InitSlot()
{
	connect(ui->ButtonOk, SIGNAL(clicked()), this, SLOT(ButtonOkClickSlot()));
}

void ThreadConfigDialog::ButtonOkClickSlot()
{
	if (ui->LineEditThread->text().toInt() == 0){
		PathManager::SetThreadNum(1);
		return;
	}
	PathManager::SetThreadNum(ui->LineEditThread->text().toInt());
}