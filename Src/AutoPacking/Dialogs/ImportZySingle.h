#ifndef IMPORT_ZY_SINGLE_H
#define IMPORT_ZY_SINGLE_H
#include <QDialog>
#include "ui_ImportZySingle.h"
#include "Parsers/XlsxParser.h"

class ImportZySingle:public QDialog
{
	Q_OBJECT
public:
	explicit ImportZySingle(QWidget *parent = NULL);
	~ImportZySingle();
protected:
	void InitView();
	void InitSlot();
private:
	Ui::ImportZySingle *ui;
	QString mxlsxName;
	XlsxParser *mpaser;
private slots:
void ButtonScanSlot();
void ButtonOkSlot();
};
#endif