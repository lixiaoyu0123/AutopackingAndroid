#ifndef IMPORT_DATA_H
#define IMPORT_DATA_H

#include <QtWidgets/QDialog>
#include "ui_ImportData.h"

class ImportData:public QDialog
{
	Q_OBJECT
public:
	explicit ImportData(QWidget *parent = 0);
	~ImportData();
protected:
private:
	Ui::ImportData *ui;
	QString mimportDataFile;
private:
	void InitSlot();
private slots:
void ButtonScanClickSlot();
void ButtonOkClickSlot();
};


#endif