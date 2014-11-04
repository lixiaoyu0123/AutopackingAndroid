#include "Dialogs/Donate.h"

Donate::Donate(QWidget *parent)
: QDialog(parent),
ui(new Ui::Donate)
{
	ui->setupUi(this);
}

Donate::~Donate()
{
	delete ui;
}
