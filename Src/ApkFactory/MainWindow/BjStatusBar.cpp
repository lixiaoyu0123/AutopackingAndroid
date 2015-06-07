#include "BjStatusBar.h"

BjStatusBar::BjStatusBar(QWidget *parent)
:QStatusBar(parent),
mlableTime(parent),
mtext1(parent)
{
	InitView();
}

void BjStatusBar::InitView()
{
	mlableTime.setAlignment(Qt::AlignCenter);
	mlableTime.setMinimumSize(mlableTime.sizeHint());
	this->setSizeGripEnabled(false); //设置是否显示右边的大小控制点
	this->addWidget(&mlableTime);
	this->addPermanentWidget(&mlableTime); //显示永久信息

	mtext1.setAlignment(Qt::AlignCenter);
	mtext1.setMinimumSize(mtext1.sizeHint());
	this->setStyleSheet(QString("QStatusBar::item{border: 0px}")); // 设置不显示label的边框
	this->addWidget(&mtext1);
}

void BjStatusBar::SetText1(QString &text)
{
	mtext1.setText(text);
}

void BjStatusBar::ShowTime(QString time)
{
	mlableTime.setText(time);
}