#ifndef EXPORT_DATA_H
#define EXPORT_DATA_H

#include <QtWidgets/QDialog>
#include "ui_ExportData.h"

class ExportData :public QDialog
{
	Q_OBJECT
public:
	explicit ExportData(QWidget *parent = 0);
	~ExportData();
protected:
private:
	Ui::ExportData *ui;
	QString mexportDataFile;
private:
	void InitSlot();
private slots:
void ButtonOkClickSlot();
void ButtonScanClickSlot();
};


#endif