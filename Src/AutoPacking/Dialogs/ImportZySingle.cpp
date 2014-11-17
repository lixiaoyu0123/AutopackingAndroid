#include <QFileDialog>
#include "ImportZySingle.h"
#include "Model/DatabaseManager.h"
#include "Model/PathManager.h"
#include "Dialogs/BjMessageBox.h"


ImportZySingle::ImportZySingle(QWidget *parent /* = NULL */) :QDialog(parent),
ui(new Ui::ImportZySingle),
mpaser(NULL)
{
	ui->setupUi(this);
	InitSlot();
}

ImportZySingle::~ImportZySingle()
{
	delete ui;
	if (mpaser != NULL){
		delete mpaser;
	}
}

void ImportZySingle::InitView()
{

}

void ImportZySingle::InitSlot()
{
	connect(ui->ButtonScan, SIGNAL(clicked()), this, SLOT(ButtonScanSlot()));
	connect(ui->ButtonOk, SIGNAL(clicked()), this, SLOT(ButtonOkSlot()));
}

void ImportZySingle::ButtonScanSlot()
{
	QString defaultPath = PathManager::ReadLastPath(QStringLiteral("importZySingle"));
	if (defaultPath.isEmpty()){
		defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	}
	QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("打开文件"), defaultPath, QStringLiteral("Excel文件 (*.xlsx)"));
	if (!fileName.isEmpty()){
		ui->LineEditXlsx->setText(fileName);
		mxlsxName = fileName;
		PathManager::WriteLastPath(QStringLiteral("importZySingle"), fileName.left(fileName.replace("\\", "/").lastIndexOf("/")));
		mpaser = new XlsxParser(mxlsxName);
		QStringList sheets = mpaser->GetAllSheet();
		if (sheets.isEmpty()){
			BjMessageBox::warning(NULL, QStringLiteral("错误!"), QStringLiteral("Excel打开失败"), QMessageBox::Ok, QMessageBox::NoButton);
			return;
		}
		for (QStringList::iterator ite = sheets.begin(); ite != sheets.end(); ite++)
		{
			ui->ComboBoxSheet->addItem(*ite);
		}
		ui->ComboBoxSheet->setCurrentIndex(0);
	}
}

void ImportZySingle::ButtonOkSlot()
{
	if (mpaser == NULL){
		BjMessageBox::information(NULL, QStringLiteral("友情提示!"), QStringLiteral("请先选择正确的Excel文件"), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	QString channelId = PathManager::GetChannelId();
	QString baseNm = PathManager::GetBaseNm();
	QString originalNm = PathManager::GetOriginalNm();
	QString appNmFile = PathManager::GetAppNmFile();
	QString oriAppNm = PathManager::GetOriAppNm();
	QString newAppNm = PathManager::GetNewAppNm();
	if (channelId.isEmpty()){
		BjMessageBox::information(NULL, QStringLiteral("友情提示!"), QStringLiteral("单本书渠道id未设置，请先设置渠道id"), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	else if (baseNm.isEmpty()){
		BjMessageBox::information(NULL, QStringLiteral("友情提示!"), QStringLiteral("单本书包名前缀未设置，请先设置包名前缀"), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	else if (originalNm.isEmpty()){
		BjMessageBox::information(NULL, QStringLiteral("友情提示!"), QStringLiteral("单本书原始包名未设置，请先设置原始包名"), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	else if (appNmFile.isEmpty()){
		BjMessageBox::information(NULL, QStringLiteral("友情提示!"), QStringLiteral("单本书应用名文件未设置，请先设置应用名文件"), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	else if (oriAppNm.isEmpty()){
		BjMessageBox::information(NULL, QStringLiteral("友情提示!"), QStringLiteral("单本书应用名原字符串未设置，请先设置应用名原字符串"), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	else if (newAppNm.isEmpty()){
		BjMessageBox::information(NULL, QStringLiteral("友情提示!"), QStringLiteral("单本书应用名新字符串未设置，请先设置应用名新字符串"), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	DatabaseManager::GetInstance()->ImportFromXlsx(*mpaser, ui->ComboBoxSheet->currentText(), 
	channelId,
	baseNm,
	originalNm,
	appNmFile,
	oriAppNm,
	newAppNm
		);
}