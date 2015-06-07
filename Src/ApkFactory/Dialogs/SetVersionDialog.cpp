#include "SetVersionDialog.h"
#include "Model/PathManager.h"

SetVersionDialog::SetVersionDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::SetVersionDialog)
{
	ui->setupUi(this);
	InitView();
	InitSlot();
}

void SetVersionDialog::InitView()
{
	ui->LineEditVersion->setText(PathManager::GetVersion());
}

void SetVersionDialog::InitSlot()
{
	connect(ui->ButtonOk, SIGNAL(clicked()), this, SLOT(ButtonOkClickSlot()));
}

void SetVersionDialog::ButtonOkClickSlot()
{
	//if (!ui->LineEditVersion->text().isEmpty()){
		PathManager::SetVersion(ui->LineEditVersion->text());
	//}
}