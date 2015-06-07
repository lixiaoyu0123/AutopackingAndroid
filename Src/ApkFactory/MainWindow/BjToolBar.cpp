#include <QFileDialog>
#include <QStandardPaths>
#include "BjToolBar.h"
#include "Model/PathManager.h"

BjToolBar::BjToolBar(QWidget *parent)
:QToolBar(parent),
mcombox(parent),
mlablePath(parent),
mlineedit(parent),
mbuttonScan(parent)
{
	InitView();
	InitSlot();
}

void BjToolBar::InitView()
{
	mpactionStar = new QAction(this);
	mpactionStop = new QAction(this);
	mpactionStar->setIconText(QStringLiteral("开始"));
	mpactionStop->setIconText(QStringLiteral("停止"));
	mpactionStar->setEnabled(true);
	mpactionStop->setDisabled(true);
	/*索引为0 表示源码打包*/
	mcombox.addItem(QStringLiteral("源码打包"));
	mcombox.addItem(QStringLiteral("反编译打包"));
	mcombox.setCurrentIndex(PathManager::GetPackWay());
	this->setFixedHeight(30);
	this->addWidget(&mcombox);
	this->addSeparator();
	mlablePath.setText(QStringLiteral("输出路径"));
	this->addWidget(&mlablePath);
	mlineedit.setMaximumHeight(23);
	mlineedit.setMaximumWidth(150);
	mlineedit.setText(PathManager::GetOutPath());
	this->addWidget(&mlineedit);
	mbuttonScan.setText(QStringLiteral("浏览"));
	mbuttonLog.setText(QStringLiteral("日志"));
	mbuttonThreadConfig.setText(QStringLiteral("线程配置"));
	this->addWidget(&mbuttonScan);
	this->addSeparator();
	this->addAction(mpactionStar);
	this->addSeparator();
	this->addAction(mpactionStop);
	this->addSeparator();
	this->addWidget(&mbuttonLog);
	this->addSeparator();
	this->addWidget(&mbuttonThreadConfig);
}

void BjToolBar::InitSlot()
{
	connect(&mbuttonScan, SIGNAL(clicked()), this, SLOT(ButtonScanClickSlot()));
	connect(&mcombox, SIGNAL(currentIndexChanged(int)), this, SLOT(ComIndexChangSlot(int)));
}

void BjToolBar::ButtonScanClickSlot()
{
	QString defaultPath = PathManager::ReadLastPath(QStringLiteral("toolbar"));
	if (defaultPath.isEmpty()){
		defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	}
	QString	dir = QFileDialog::getExistingDirectory(this, QStringLiteral("选择路径"), defaultPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty()){ 
		PathManager::SetOutPath(dir);
		PathManager::WriteLastPath(QStringLiteral("toolbar"), dir);
		mlineedit.setText(dir);
	}
}

QAction *BjToolBar::GetActionStar()
{
	return mpactionStar;
}

QAction *BjToolBar::GetActionStop()
{
	return mpactionStop;
}

QComboBox *BjToolBar::GetCombox()
{
	return &mcombox;
}

QPushButton *BjToolBar::GetButtonLog()
{
	return &mbuttonLog;
}

QPushButton *BjToolBar::GetButtonThreadConfig()
{
	return &mbuttonThreadConfig;
}

QPushButton *BjToolBar::GetButtonScan()
{
	return &mbuttonScan;
}

void BjToolBar::ComIndexChangSlot(int index)
{
	PathManager::SetPackWay(index);
}