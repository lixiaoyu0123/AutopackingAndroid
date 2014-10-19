#include <QMessageBox>
#include "LogDialog.h"

LogDialog::LogDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::LogDialog)
{
	ui->setupUi(this);
}

LogDialog::~LogDialog()
{
	delete ui;
}

void LogDialog::SetText(QString &text)
{
	ui->TextBrowserLog->append(text);
}