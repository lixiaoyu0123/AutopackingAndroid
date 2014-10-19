#ifndef SOURCE_PACK_SETTING_H
#define SOURCE_PACK_SETTING_H

#include <QtWidgets/QDialog>
#include "ui_SourcePackSetting.h"

class SourcePackSetting :public QDialog
{
	Q_OBJECT
public:
	explicit SourcePackSetting(QWidget *parent = 0);
	~SourcePackSetting();
protected:
private:
	Ui::SourcePackSetting *ui;
private:
	void InitSlot();
	void InitView();
private slots:
void ButtonOkClickSlot();
void ButtonScanClickSlot();
void ButtonScanSdkClickSlot();
};


#endif