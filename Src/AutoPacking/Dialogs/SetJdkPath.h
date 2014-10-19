#ifndef SET_JDK_PATH_H
#define SET_JDK_PATH_H

#include <QtWidgets/QDialog>
#include "ui_SetJdkPath.h"

class SetJdkPath :public QDialog
{
	Q_OBJECT
public:
	explicit SetJdkPath(QWidget *parent = 0);
	~SetJdkPath();
protected:
private:
	Ui::SetJdkPath *ui;
	QString mjdkPath;
private:
	void InitSlot();
	void InitView();
	private slots:
	void ButtonScanClickSlot();
	void ButtonOkClickSlot();
};


#endif