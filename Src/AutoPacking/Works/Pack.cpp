#include <QDir>
#include <QTextCodec>
#include "Pack.h"
#include "Model/PathManager.h"
#include "Custom/ZySingleBook.h"

Pack::Pack(QObject *parent):
QThread(parent),
moutFile(""),
mtmpSignFile(""),
mchannelId(""),
mchannelName(""),
mtaskId(0),
mpprocess(NULL),
mtmpCodePath("")
{

}

Pack::~Pack()
{
	if (mpprocess != NULL){
		mpprocess->terminate();
		mpprocess->deleteLater();
		mpprocess = NULL;
	}
	quit();
	wait();
}

void Pack::Stop()
{
	if (mpprocess != NULL){
		QProcess killer;
		killer.start("taskkill", QStringList() << "/f" << "/im" << "java.exe");
		if (!killer.waitForStarted())
			return;
		if (!killer.waitForFinished())
			return;
		mpprocess->terminate();
		mpprocess->deleteLater();
		mpprocess = NULL;
	}
	this->terminate();
	this->wait();
	if (!PathManager::RemoveDir(mtmpPath)){
		emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
	}
	emit FinishSignal(2, mtaskId);
}

int Pack::GetTaskId()
{
	return mtaskId;
}

bool Pack::ReplaceStrByTable(QString &path)
{
	for (QList<ReplaceStrTable>::iterator ite = mstrTableList.begin(); ite != mstrTableList.end(); ite++)
	{
		QString findFile;
		QString file = ite->GetFolder();
		if (file.isEmpty() || ite->GetSrcStr().isEmpty() || ite->GetDestStr().isEmpty()){
			emit GenerateError(QStringLiteral("error:要替换字符串的文件不存在或文件或替换的字符串或目标字符产为空！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}

		if (file.replace("\\", "/").startsWith("/")){
			findFile = path + file;
		}
		else{
			findFile = path + "/" + file;
		}

		if (!QFile::exists(findFile)){
			emit GenerateError(QStringLiteral("error:要替换字符串的文件不存在！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}

		switch (PathManager::ReplaceStrStrict(findFile, ite->GetSrcStr(), ite->GetDestStr()))
		{
		case 1:
			emit GenerateError(QStringLiteral("error:替换字符串失败！未发现原字符串在指定的文件中！ 渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		case 2:
			emit GenerateError(QStringLiteral("error:替换字符串失败！指定文件开错误！ 渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		case 3:
			emit GenerateError(QStringLiteral("error:替换字符串失败！其他错误！ 渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}
	}
	return true;
}

bool Pack::ReplaceResByTable(QString &path)
{
	for (QList<ReplaceResTable>::iterator ite = mresTableList.begin(); ite != mresTableList.end(); ite++)
	{
		QString dirDestAbsolutely;
		QString dirSrc;
		QString dirDest;
		dirSrc = ite->GetFolderSrc();
		dirDest = ite->GetFolderDest();
		if (dirSrc.isEmpty() || dirDest.isEmpty()){
			emit GenerateError(QStringLiteral("error:原资源文件夹或目标文件夹未配置！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}
		if (dirSrc.replace("\\", "/").startsWith("/")){
			dirDestAbsolutely = path + dirDest;
		}
		else{
			dirDestAbsolutely = path + "/" + dirDest;
		}
		QDir tmp(dirSrc);
		if (!tmp.exists()){
			emit GenerateError(QStringLiteral("error:资源原路径不存在！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}
		PathManager::CopyDir(dirSrc, dirDestAbsolutely, true);
	}
	return true;
}

bool Pack::ExecuteCmd(QString exe, QStringList argument, QProcess &pprocess, QString workPath)
{
	QString enterPath = QStringLiteral("cd ") + "\"" + workPath + "\"" + QStringLiteral("\n");
	QString arg;
	arg.append(exe);
	for (QStringList::Iterator ite = argument.begin(); ite != argument.end(); ite++)
	{
		arg.append(" ");
		arg.append(*ite);		
	}
	arg.append("\n");
	pprocess.start("cmd");
	pprocess.waitForStarted();
	QTextCodec *gbk = QTextCodec::codecForName("GBK");
	QByteArray byteEnterPath = gbk->fromUnicode(enterPath.constData(), enterPath.length());
	QByteArray byteCommand = gbk->fromUnicode(arg.constData(), arg.length());
	char *charEnterPath = byteEnterPath.data();
	char *charCommand = byteCommand.data();
	pprocess.write(byteEnterPath);
	pprocess.write(charCommand);
	pprocess.closeWriteChannel();
	if (!pprocess.waitForFinished(1000 * 60 * 20)){
		return false;
	}
	return true;
	//mpprocess->setWorkingDirectory(workPath);
	//mpprocess->start(exe,argument);
}

bool Pack::CheckError(QProcess &pprocess)
{
	QTextCodec *gbk = QTextCodec::codecForName("GBK");
	QString error = gbk->toUnicode(pprocess.readAllStandardError());
	if (error.toLower().contains("error:") 
		|| error.toLower().contains("exception:")
		|| error.toLower().contains(" error")
		|| error.toLower().contains(" exception")
		|| error.toLower().contains("build failed")){
		QString standardOut = gbk->toUnicode(pprocess.readAllStandardOutput());
		emit GenerateError(error.append(standardOut));
		return false;
	}
	return true;
}

bool Pack::ZySingle()
{
	bool isMakeIcon;
	ZySingleBook singleBook(NULL, mchannelId, mtmpCodePath);
	if (!singleBook.MakeBook(isMakeIcon)){
		return false;
	}
	if (isMakeIcon){
		emit GenerateError(QStringLiteral("warn:未找到Icon在资源路径，使用了封面生成Icon！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
	}
	return true;
}