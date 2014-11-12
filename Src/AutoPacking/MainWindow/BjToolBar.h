#ifndef BJTOOLBAR_H
#define BJTOOLBAR_H
#include <QToolBar>
#include <QComboBox>
#include <QAction>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class BjToolBar :public QToolBar
{
	Q_OBJECT
private:
	QComboBox mcombox;
	QComboBox mcomboxCustom;
	QLineEdit mlineedit;
	QLabel mlableWay;
	QLabel mlableCustom;
	QLabel mlablePath;
	QPushButton mbuttonScan;
	QPushButton mbuttonLog;
	QPushButton mbuttonThreadConfig;
	QAction *mpactionStar;
	QAction *mpactionStop;

public:
	explicit BjToolBar(QWidget *parent = 0);
	void InitView();
	void InitSlot();
	QAction *GetActionStar();
	QAction *GetActionStop();
	QPushButton *GetButtonLog();
	QPushButton *GetButtonThreadConfig();
	QPushButton *GetButtonScan();
	QComboBox *GetCombox();
	private slots :
	void ButtonScanClickSlot();
	void ComIndexChangSlot(int index);
	void CustomComIndexChangSlot(int index);
};
#endif