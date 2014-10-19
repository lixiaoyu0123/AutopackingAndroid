#ifndef BJSTATUSBAR_H
#define BJSTATUSBAR_H
#include <QStatusBar>
#include <QLabel>

class BjStatusBar:public QStatusBar
{
	Q_OBJECT
public:
	explicit BjStatusBar(QWidget *parent = 0);
	void ShowTime(QString time);
	void InitView();
	void SetText1(QString &text);
protected:
private:
	QLabel mlableTime;
	QLabel mtext1;
};
#endif