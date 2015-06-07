#ifndef DONATE_H
#define DONATE_H

#include <QtWidgets/QDialog>
#include "ui_Donate.h"

class Donate : public QDialog
{
	Q_OBJECT

public:

	/**
	* Constructor.
	*
	* @param parent The dialog's parent.
	*/
	explicit Donate(QWidget *parent = 0);
	~Donate();

private:
	Ui::Donate *ui;
};
#endif