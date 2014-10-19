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
	QList<ReplaceStrTable> &strTableList, QList<ReplaceResTable> &resTableList, int taskId)
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
	QString exePath = apkTool.left(apkTool.lastIndexOf("/"));
	ExecuteCmd(apkTool, param, exePath);
}

void DecPack::Dopacket(QString &inPath, QString &outPath)
{
	QString apkTool = PathManager::GetApkToolPath();
	QStringList param;
	param << QString("b") << inPath << outPath;
	QString exePath = apkTool.left(apkTool.lastIndexOf("/"));
	ExecuteCmd(apkTool, param, exePath);
}

void DecPack::SignPacket(QString inPath, QString outPath)
{
	QString exe = PathManager::GetJarSigner();
	QStringList param;
	param << QStringLiteral("-sigalg") << PathManager::GetSigalg() << QStringLiteral("-verbose") << QStringLiteral("-digestalg")
		<< PathManager::GetDigestalg() << QStringLiteral("-keystore") << PathManager::GetKeyPath().insert(0,"\"").append("\"") << QStringLiteral("-storepass") << PathManager::GetPasswd()
		<< QStringLiteral("-keypass") << PathManager::GetPasswd() << outPath.insert(0, "\"").append("\"") << PathManager::GetKeyAliases().trimmed();
	QString exePath = exe.left(exe.lastIndexOf("/"));
	ExecuteCmd(exe, param, exePath);
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
	param << QStringLiteral("-v") << QStringLiteral("4") << mtmpSignFile << moutFile;
	QString exePath = exe.left(exe.lastIndexOf("/"));
	ExecuteCmd(exe, param, exePath);
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