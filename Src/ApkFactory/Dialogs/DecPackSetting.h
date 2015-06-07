#ifndef DEC_PACK_SETTING_H
#define DEC_PACK_SETTING_H

#include <QtWidgets/QDialog>
#include "ui_DecPackSetting.h"

class DecPackSetting :public QDialog
{
	Q_OBJECT
public:
	explicit DecPackSetting(QWidget *parent = 0);
	~DecPackSetting();
protected:
private:
	Ui::DecPackSetting *ui;
	QString mtemplate;
private:
	void InitSlot();
	void InitView();
private slots:
	void ButtonScanClickSlot();
	void ButtonOkClickSlot();
};


#endif