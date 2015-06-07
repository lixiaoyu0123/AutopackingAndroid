#ifndef SHOW_LOG_H
#define SHOW_LOG_H

#include <QtWidgets/QDialog>
#include "ui_LogDialog.h"

class LogDialog :public QDialog
{
	Q_OBJECT
public:
	explicit LogDialog(QWidget *parent = 0);
	~LogDialog();
	void SetText(QString &text);
protected:
private:
	Ui::LogDialog *ui;
};


#endif