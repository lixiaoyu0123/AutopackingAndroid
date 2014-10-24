#include <QDir>
#include <QTextCodec>
#include "Pack.h"
#include "Model/PathManager.h"

Pack::Pack(QObject *parent):
QObject(parent),
moutFile(""),
mtmpSignFile(""),
mchannelId(""),
mchannelName(""),
mtaskId(0)
{

}

Pack::~Pack()
{

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

		if (!PathManager::ReplaceStr(findFile, ite->GetSrcStr(), ite->GetDestStr())){
			emit GenerateError(QStringLiteral("error:替换字符串失败！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}
	}
	return true;
}

bool Pack::ReplaceResByTable(QString &path)
{
	for (QList<ReplaceResTable>::iterator ite = mresTableList.begin(); ite != mresTableList.end(); ite++)
	{
		QString dirSrcAbsolutely;
		QString dirSrc;
		QString dirDest;
		dirSrc = ite->GetFolderSrc();
		dirDest = ite->GetFolderDest();
		if (dirSrc.isEmpty() || dirDest.isEmpty()){
			emit GenerateError(QStringLiteral("error:原资源文件夹或目标文件夹未配置！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}
		if (dirSrc.replace("\\", "/").startsWith("/")){
			dirSrcAbsolutely = path + dirSrc;
		}
		else{
			dirSrcAbsolutely = path + "/" + dirSrc;
		}
		QDir tmp(dirDest);
		if (!tmp.exists()){
			emit GenerateError(QStringLiteral("error:资源原路径不存在！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}
		PathManager::CopyDir(dirDest, dirDest, true);
	}
	return true;
}

void Pack::ExecuteCmd(QString exe, QStringList argument, QString workPath)
{
	//QString exePath = QStringLiteral("cd ") + exe.left(exe.lastIndexOf("/")) + QStringLiteral("\n");
	//QString exeName = exe.mid(exe.lastIndexOf("/") + 1);
	//QString arg;
	//arg.append(exeName);
	//for (QStringList::Iterator ite = argument.begin(); ite != argument.end(); ite++)
	//{
	//	arg.append(" ");
	//	arg.append(*ite);		
	//}
	//arg.append("\n");
	//mpprocess->start("cmd");
	//mpprocess->waitForStarted();
	//QTextCodec *gbk = QTextCodec::codecForName("GBK");
	//QByteArray byteToolPath = gbk->fromUnicode(exePath.constData(), exePath.length());
	//QByteArray byteArg = gbk->fromUnicode(arg.constData(), arg.length());
	//char *toolPathChar = byteToolPath.data();
	//char *argChar = byteArg.data();
	//mpprocess->write(toolPathChar);
	//mpprocess->write(argChar);
	//mpprocess->closeWriteChannel();
	//mpprocess->waitForFinished();
	//QString strOut = QString::fromLocal8Bit(mpprocess->readAllStandardOutput());
	//QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	//env.insert("PATH", env.value("Path") + exePath);
	//mpprocess->setProcessEnvironment(env);

	//mpprocess->setProcessChannelMode(QProcess::MergedChannels);

	mpprocess->setWorkingDirectory(workPath);
	mpprocess->start(exe,argument);
}

bool Pack::CheckError()
{
	QTextCodec *gbk = QTextCodec::codecForName("GBK");
	QString error = gbk->toUnicode(mpprocess->readAllStandardError());
	if (error.toLower().contains("error:") 
		|| error.toLower().contains("exception:")
		|| error.toLower().contains(" error")
		|| error.toLower().contains(" exception")){
		emit GenerateError(error);
		return false;
	}
	return true;
}