#ifndef SETVERSION_DIALOG_H
#define SETVERSION_DIALOG_H
#include <QDialog>
#include "ui_SetVersionDialog.h"

class SetVersionDialog:public QDialog
{
	Q_OBJECT
public:
	SetVersionDialog(QWidget *parent = 0);

protected:
private:
	Ui::SetVersionDialog *ui;
private slots:
void ButtonOkClickSlot();
private:
	void InitView();
	void InitSlot();
};
#endif