#ifndef ZYSINGLESETTING_H
#define ZYSINGLESETTING_H
#include <QDialog>
#include "ui_ZySingleSetting.h"

class ZySingleSetting:public QDialog
{
	Q_OBJECT
public:
	explicit ZySingleSetting(QWidget *parent = NULL);
	~ZySingleSetting();
protected:
private:
	Ui::ZySingleSetting *ui;

private:
	void InitView();
	void InitSlot();

private slots:
	void ButtonOkSlot();
	//void ButtonScanSlot();

};
#endif