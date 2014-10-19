#ifndef THREAD_CONFIG_DIALOG_H
#define THREAD_CONFIG_DIALOG_H
#include <QDialog>
#include "ui_ThreadConfigDialog.h"

class ThreadConfigDialog:public QDialog
{
	Q_OBJECT
public:
	explicit ThreadConfigDialog(QWidget *parent = 0);
	~ThreadConfigDialog();
protected:
private:
	Ui::ThreadConfigDialog *ui;
private:
	void InitView();
	void InitSlot();
private slots:
void ButtonOkClickSlot();
};
#endif