#ifndef SET_KEY_H
#define SET_KEY_H

#include <QtWidgets/QDialog>
#include "ui_SetKey.h"

class SetKey :public QDialog
{
	Q_OBJECT
public:
	explicit SetKey(QWidget *parent = 0);
	~SetKey();
protected:
private:
	Ui::SetKey *ui;
	QString mkeyPath;
	private slots:
	void ChangStatusSlot();
	void ButtonScanClickSlot();
	void ButtonOkClickSlot();
private:
	void InitSlot();
	void InitView();
};


#endif