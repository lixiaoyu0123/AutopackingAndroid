#include <QDir>
#include "SrcPack.h"
#include "Model/PathManager.h"

SrcPack::SrcPack(QObject *parent):
Pack(parent)
{
	mpprocess = new QProcess(this);
}

SrcPack::~SrcPack()
{
	mpprocess->deleteLater();
}

void SrcPack::Start(QString &inPath, QString &outPath, QString &channelId, QString &channelName, QString &channeltbID,
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
	CreatPath(outPath, channelId, channelName, channeltbID);
	if (!CopySrc(PathManager::GetSrcPath(), mtmpSrcPath)){
		return;
	}

	QFile buildXml(mtmpSrcPath + QStringLiteral("/build.xml"));
	if (buildXml.exists() && !buildXml.remove()){
		return;
	}
	PrePack();
}

void SrcPack::Stop()
{
	if (mpprocess->state() == QProcess::Running){
		mpprocess->close();
	}
	if (!PathManager::RemoveDir(mtmpPath)){
		emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
	}
	emit FinishSignal(2, mtaskId);
}

void SrcPack::CreatPath(QString &outPath, QString &channelId, QString &channelName, QString &channeltbId)
{
	if (outPath.endsWith("/")){
		moutFile = outPath + channelName + "_" + channelId + "_" + PathManager::GetVersion() + ".apk";
	}
	else{
		moutFile = outPath + "/" + channelName + "_" + channelId + "_" + PathManager::GetVersion() + ".apk";
	}

	mtmpPath = PathManager::GetTmpPath() + QStringLiteral("/") + channeltbId;
	QString srcPath = mtmpPath + QStringLiteral("/src");
	QString signPath = mtmpPath + QStringLiteral("/sign");
	PathManager::CreatDir(srcPath);
	PathManager::CreatDir(signPath);
	mtmpSrcPath = srcPath;
	mtmpSignFile = signPath + "/" + channelName + "_" + channelId + "_" + PathManager::GetVersion() + ".apk";
}

bool SrcPack::CopySrc(QString &srcPath, QString &destPath)
{
	QDir dir(destPath);
	if (dir.exists()){
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除上次缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
	}
	return PathManager::CopyDir(srcPath, destPath,true);
}

void SrcPack::PrePack()
{
	connect(mpprocess, SIGNAL(finished(int)), this, SLOT(PrePackFinishedSlot(int)));
	QString target = PathManager::GetTarget(mtmpSrcPath);
	if (target.isEmpty()){
		emit GenerateError(QStringLiteral("error:未找到project.properties文件或者未指定target"));
		return;
	}
	QString prepackBat = PathManager::GetPrePack();
	QStringList param;
	target = target.remove("").remove("target=android-");
	param << PathManager::GetAndroid() << PathManager::GetSdkToolsPath() << QString::number(target.toInt() - 13) << mtmpSrcPath;
	QString content = QStringLiteral("\nkey.store=%1\nkey.alias=%2\nkey.store.password=%3\nkey.alias.password=%4\n")
		.arg(PathManager::GetKeyPath())
		.arg(PathManager::GetKeyAliases())
		.arg(PathManager::GetPasswd())
		.arg(PathManager::GetPasswd());
	PathManager::AppendContentToProperties(content, mtmpSrcPath);
	ExecuteCmd(prepackBat, param, PathManager::GetSdkPath());
}

void SrcPack::PackFromSrc()
{
	connect(mpprocess, SIGNAL(finished(int)), this, SLOT(PackFromSrcFinishedSlot(int)));
	QString srcpackBat = PathManager::GetSrcPack();
	QStringList param;
	param << PathManager::GetAnt() << mtmpSrcPath;
	ExecuteCmd(srcpackBat, param, PathManager::GetAntPath());
}

void SrcPack::PrePackFinishedSlot(int stat)
{
	disconnect(mpprocess, SIGNAL(finished(int)), this, SLOT(PrePackFinishedSlot(int)));
	if (!ReplaceStrByTable(mtmpSrcPath)){
		emit GenerateError(QStringLiteral("error:替换字符串出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
	}

	if (!ReplaceResByTable(mtmpSrcPath)){
		emit GenerateError(QStringLiteral("error:替换资源出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
	}
	PackFromSrc();
}

void SrcPack::PackFromSrcFinishedSlot(int stat)
{
	disconnect(mpprocess, SIGNAL(finished(int)), this, SLOT(PackFromSrcFinishedSlot(int)));
	QString apk = PathManager::GetReleaseApk(PathManager::GetBin(mtmpSrcPath));
	if (apk.isEmpty()){
		return;
	}
	if (!QFile::copy(PathManager::GetBin(mtmpSrcPath) + QString("/") + apk, moutFile)){
		emit GenerateError(QStringLiteral("error:拷贝签名文件出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
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