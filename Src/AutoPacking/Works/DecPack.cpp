#include <QProcess>
#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include "DecPack.h"
#include "Model/PathManager.h"

DecPack::DecPack(QObject *parent) :Pack(parent),
mtmpUnpacketPath("")
{
	mpprocess = new QProcess(this);
}

DecPack::~DecPack()
{
	mpprocess->deleteLater();
}

void DecPack::Start(QString &inPath, QString &outPath, QString &channelId, QString &channelName, QString &channeltbID,
	QList<ReplaceStrTable> &strTableList, QList<ReplaceResTable> &resTableList, QList<ReplacePakTable> &pakTableList, int taskId)
{
	if (inPath.isEmpty() || outPath.isEmpty()){
		emit GenerateError(QStringLiteral("error:输入输出路径为空！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		emit FinishSignal(1, mtaskId);
		return;
	}

	if (channelId.isEmpty() && channelName.isEmpty()){
		emit GenerateError(QStringLiteral("error:渠道id和渠道名都为空！渠道ID: %1, 渠道名 : %2\n").arg(mchannelId).arg(mchannelName));
		emit FinishSignal(1, mtaskId);
		return;
	}

	mchannelId = channelId;
	mchannelName = channelName;
	mtaskId = taskId;
	mstrTableList = strTableList;
	mresTableList = resTableList;
	mpakTableList = pakTableList;
	CreatPath(outPath,channelId, channelName, channeltbID);
	Unpacket(inPath, mtmpUnpacketPath);
}

void DecPack::Stop()
{
	if (mpprocess->state() == QProcess::Running){
		mpprocess->close();
	}
	if (!PathManager::RemoveDir(mtmpPath)){
		emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
	}
	emit FinishSignal(2, mtaskId);
}

void DecPack::CreatPath(QString &outPath, QString &channelId, QString &channelName, QString &channeltbId)
{
	if (outPath.endsWith("/")){
		moutFile = outPath + channelName + "_" + channelId + "_" + PathManager::GetVersion() + ".apk";
	}
	else{
		moutFile = outPath + "/" + channelName + "_" + channelId + "_" + PathManager::GetVersion() + ".apk";
	}
	
	mtmpPath = PathManager::GetTmpPath() + QStringLiteral("/") + channeltbId;
	QString unpackPath = mtmpPath + QStringLiteral("/unpack");
	QString signPath = mtmpPath + QStringLiteral("/sign");
	PathManager::CreatDir(unpackPath);
	PathManager::CreatDir(signPath);
	mtmpUnpacketPath = unpackPath;
	mtmpSignFile = signPath + "/" + channelName + "_" + channelId + "_" + PathManager::GetVersion() + ".apk";
}

bool DecPack::ReplacePakByTable()
{
	for (QList<ReplacePakTable>::iterator ite = mpakTableList.begin(); ite != mpakTableList.end(); ite++)
	{
		switch (PathManager::ReplacePakInDec(mtmpUnpacketPath, ite->GetSrcPakName(), ite->GetDestPakName()))
		{
		case 0:
			break;
		case 1:
			emit GenerateError(QStringLiteral("error:替换包名出错,原包名不存在！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		case 2:
			emit GenerateError(QStringLiteral("error:替换包名出错,创建包出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		case 3:
			emit GenerateError(QStringLiteral("error:替换包名出错,目的包名已经存在！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		case 4:
			emit GenerateError(QStringLiteral("error:替换包名出错,替换包名过程出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}
	}
	return true;
}

void DecPack::Unpacket(QString &inPath, QString &outPath)
{
	QDir dir(outPath);
	if (dir.exists()){
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除上次缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
	}
	connect(mpprocess, SIGNAL(finished(int)), this, SLOT(UnpacketFinishedSlot(int)));
	QString apkTool = PathManager::GetApkToolPath();
	QStringList param;
	param << QString("d") << QString("-f") << inPath << outPath;
	ExecuteCmd(apkTool, param, PathManager::GetToolPath());
}

void DecPack::Dopacket(QString &inPath, QString &outPath)
{
	QString apkTool = PathManager::GetApkToolPath();
	QStringList param;
	param << QString("b") << inPath << outPath;
	ExecuteCmd(apkTool, param, PathManager::GetToolPath());
}

void DecPack::SignPacket(QString inPath, QString outPath)
{
	QString exe = PathManager::GetJarSigner();
	QStringList param;
	param << QStringLiteral("-sigalg") << PathManager::GetSigalg() << QStringLiteral("-verbose") << QStringLiteral("-digestalg")
		<< PathManager::GetDigestalg() << QStringLiteral("-keystore") << PathManager::GetKeyPath().insert(0,"\"").append("\"") << QStringLiteral("-storepass") << PathManager::GetPasswd()
		<< QStringLiteral("-keypass") << PathManager::GetAliasesPasswd() << outPath.insert(0, "\"").append("\"") << PathManager::GetKeyAliases().trimmed();
	//ExecuteCmd(exe, param, PathManager::GetJdkPath());
	QString arg;
	for (QStringList::Iterator ite = param.begin(); ite != param.end(); ite++)
	{
		arg.append(" ");
		arg.append(*ite);		
	}
	mpprocess->start("cmd");
	mpprocess->waitForStarted();
	QTextCodec *gbk = QTextCodec::codecForName("GBK");
	QString test;
	test.append("cd ").append(PathManager::GetJdkPath().insert(0, "\"").append("\"")).append("\n");
	QByteArray byteToolPath = gbk->fromUnicode(test.constData(), test.length());
	//QByteArray byteArg = gbk->fromUnicode(arg.constData(), arg.length());
	char *toolPathChar = byteToolPath.data();
	//char *argChar = byteArg.data();
	mpprocess->write(toolPathChar);
	//mpprocess->closeWriteChannel();
	mpprocess->waitForFinished();
	QString strOut1 = QString::fromLocal8Bit(mpprocess->readAllStandardOutput());

	QString test2 = arg.insert(0, QString("jarsigner.exe ")).append("\n");
	QTextCodec *gbk2 = QTextCodec::codecForName("GBK");
	QByteArray byteToolPath2 = gbk2->fromUnicode(test2.constData(), test2.length());
	//QByteArray byteArg = gbk->fromUnicode(arg.constData(), arg.length());
	char *toolPathChar2 = byteToolPath2.data();
	mpprocess->write(toolPathChar2);
	mpprocess->waitForFinished();

	//mpprocess->write(argChar);
	//mpprocess->closeWriteChannel();
	//mpprocess->waitForFinished();
	//QString strOut2 = QString::fromLocal8Bit(mpprocess->readAllStandardOutput());
	SignFinishedSlot(0);
}

void DecPack::Zipalign()
{
	connect(mpprocess, SIGNAL(finished(int)), this, SLOT(ZipalignFinishedSlot(int)));
	if (QFile::exists(moutFile)){
		if (!QFile::remove(moutFile)){
			emit GenerateError(QStringLiteral("error:清除输入目录原包出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			if (!PathManager::RemoveDir(mtmpPath)){
				emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			}
			emit FinishSignal(1, mtaskId);
			return;
		}
	}
	QString exe = PathManager::GetZipalign();
	QStringList param;
	param << QStringLiteral("-v") << QStringLiteral("4") << mtmpSignFile.insert(0, "\"").append("\"") << moutFile.insert(0, "\"").append("\"");
	ExecuteCmd(exe.insert(0, "\"").append("\""), param, PathManager::GetToolPath());
}

void DecPack::UnpacketFinishedSlot(int stat)
{
	disconnect(mpprocess, SIGNAL(finished(int)), this, SLOT(UnpacketFinishedSlot(int)));
	if (!CheckError()){
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}

	if (!ReplaceStrByTable(mtmpUnpacketPath)){
		emit GenerateError(QStringLiteral("error:替换字符串出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
	}

	if (!ReplaceResByTable(mtmpUnpacketPath)){
		emit GenerateError(QStringLiteral("error:替换资源出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
	}

	if (!ReplacePakByTable()){
		emit FinishSignal(1, mtaskId);
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		return;
	}
	connect(mpprocess, SIGNAL(finished(int)), this, SLOT(DopacketFinishedSlot(int)));
	Dopacket(mtmpUnpacketPath, mtmpSignFile);
}

void DecPack::DopacketFinishedSlot(int stat)
{
	disconnect(mpprocess, SIGNAL(finished(int)), this, SLOT(DopacketFinishedSlot(int)));
	if (!CheckError()){
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}	
	connect(mpprocess, SIGNAL(finished(int)), this, SLOT(SignFinishedSlot(int)));
	SignPacket(mtmpSignFile, mtmpSignFile);
}

void DecPack::SignFinishedSlot(int stat)
{
	disconnect(mpprocess, SIGNAL(finished(int)), this, SLOT(SignFinishedSlot(int)));
	if (!CheckError()){
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}
	Zipalign();
}

void DecPack::ZipalignFinishedSlot(int stat)
{
	disconnect(mpprocess, SIGNAL(finished(int)), this, SLOT(ZipalignFinishedSlot(int)));
	if (!CheckError()){
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}

	if (!PathManager::RemoveDir(mtmpPath)){
		emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
	}

	emit FinishSignal(0, mtaskId);
}