#ifndef DEPACK_H
#define DEPACK_H
#include <QDialog>
#include <QProcess>
#include "ui_DePack.h"
#include "Works/UnPackApk.h"

class DePack:public QDialog
{
	Q_OBJECT
public:
	explicit DePack(QWidget *parent = NULL);
	~DePack();
protected:
private:
	Ui::DePack *ui;
	QTimer *mptimer;
private:
	void InitView();
	void InitSlot();
	void ChangStat(bool stat);
private slots:
void ButtonScanOriSlot();
void ButtonScanResultSlot();
void ButtonOkSlot();
void ButtonCancelSlot();
void UpdateProcessSlot();
void FinishedSlot();
};
#endif