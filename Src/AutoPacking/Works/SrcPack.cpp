#include <QDir>
#include "SrcPack.h"
#include "Model/PathManager.h"
#include "Model/Tools.h"

SrcPack::SrcPack(QObject *parent):
Pack(parent)
{
}

SrcPack::~SrcPack()
{
}

void SrcPack::Init(QString &inPath, QString &outPath, QString &channelId, QString &channelName, QString &channeltbID,
	QList<ReplaceStrTable> &strTableList, QList<ReplaceResTable> &resTableList, QList<ReplacePakTable> &pakTableList, QList<ReplaceAppPakTable> &appPakTableList, int taskId)
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
	mappPakTableList = appPakTableList;
	moutputPath = outPath;
	mchanneltbId = channeltbID;
}

void SrcPack::run()
{
	if (!PathManager::RemoveDir(mtmpPath)){
		emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
	}

	mpprocess = new QProcess(NULL);
	if (!CreatPath(moutputPath, mchannelId, mchannelName, mchanneltbId)){
		KillTask();
		emit FinishSignal(1, mtaskId);
		return;
	}

	QString mainProPath = mtmpSrcPath + "/" + mmainNm;

	if (!CopySrc(PathManager::GetSrcPath().trimmed(), mtmpSrcPath)){
		emit GenerateError(QStringLiteral("error:拷贝源码文件失败！渠道ID: %1, 渠道名 : %2\n").arg(mchannelId).arg(mchannelName));
		KillTask();
		emit FinishSignal(1, mtaskId);
		return;
	}

	QFile buildXml(mainProPath + QStringLiteral("/build.xml"));
	if (buildXml.exists()){
		emit GenerateError(QStringLiteral("error:buld.xml文件未删除成功！渠道ID: %1, 渠道名 : %2\n").arg(mchannelId).arg(mchannelName));
		KillTask();
		emit FinishSignal(1, mtaskId);
		return;
	}

	if (!PrePack(*mpprocess)){
		KillTask();
		emit FinishSignal(1, mtaskId);
		return;
	}

	if (!CheckError(*mpprocess)){
		KillTask();
		emit FinishSignal(1, mtaskId);
		return;
	}

	if (!PackFromSrc(*mpprocess)){
		KillTask();
		emit FinishSignal(1, mtaskId);
		return;
	}

	if (!CheckError(*mpprocess)){
		KillTask();
		emit FinishSignal(1, mtaskId);
		return;
	}

	QString apkPath;	
	apkPath = PathManager::GetApkDir(mainProPath);
	if (apkPath.isEmpty()){
		apkPath = PathManager::GetBin(mainProPath);
	}

	QString apk = PathManager::GetReleaseApk(apkPath);
	if (apk.isEmpty()){
		KillTask();
		emit GenerateError(QStringLiteral("error:打包后的apk文件未找到！渠道ID: %1, 渠道名: %2\n").arg(mchannelId).arg(mchannelName));
		emit FinishSignal(1, mtaskId);
		return;
	}
	if (!PathManager::CopyFile(apkPath + QString("/") + apk, moutFile, true)){
		KillTask();
		emit GenerateError(QStringLiteral("error:拷贝签名文件出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}

	KillTask();
	emit FinishSignal(0, mtaskId);
}

bool SrcPack::CreatPath(QString &outPath, QString &channelId, QString &channelName, QString &channeltbId)
{
	if (outPath.endsWith("/")){
		if (PathManager::GetVersion().trimmed().isEmpty()){
			moutFile = outPath + channelName + "_" + channelId + ".apk";
		}
		else{
			moutFile = outPath + channelName + "_" + PathManager::GetVersion().trimmed() + "_" + channelId + ".apk";
		}
	}
	else{
		if (PathManager::GetVersion().trimmed().isEmpty()){
			moutFile = outPath + "/" + channelName + "_" + channelId + ".apk";
		}
		else{
			moutFile = outPath + "/" + channelName + "_" + PathManager::GetVersion().trimmed() + "_" + channelId + ".apk";
		}
	}

	mtmpPath = PathManager::GetTmpPath() + QStringLiteral("/") + channeltbId;
	QString srcPath = mtmpPath + QStringLiteral("/src");
	QString signPath = mtmpPath + QStringLiteral("/sign");

	QDir tmpDir(srcPath);
	if (tmpDir.exists(srcPath)){
		if (!PathManager::RemoveDir(srcPath)){
			return false;
		}
	}

	if (tmpDir.exists(signPath)){
		if (!PathManager::RemoveDir(signPath)){
			return false;
		}
	}

	PathManager::CreatDir(srcPath);
	PathManager::CreatDir(signPath);
	mtmpSrcPath = srcPath;
	mtmpSignFile = signPath + "/" + channelName + "_" + PathManager::GetVersion() + channelId + "_" + ".apk";
	mmainNm = PathManager::GetSrcPath();
	mmainNm = mmainNm.mid(mmainNm.lastIndexOf("/") + 1);
	return true;
}

bool SrcPack::CopySrc(QString &srcPath, QString &destPath)
{
	QString desDir = destPath + "/" + srcPath.mid(srcPath.lastIndexOf("/") + 1);
	if (!PathManager::CopyDir(srcPath, desDir, true)){
		return false;
	}

	PathManager::CorrectLibRef(desDir);
	if (!PathManager::RemoveDir(desDir + "/bin")){
		return false;
	}

	QString apkStoreS = PathManager::GetKeyPath();
	QFile apkStoreF(apkStoreS);
	if (!apkStoreF.exists()){
		return false;
	}

	QFile newF(destPath + "/apk.keystore");
	if (newF.exists()){
		if (!newF.remove()){
			return false;
		}
	}

	if (!QFile::copy(apkStoreF.fileName(), newF.fileName())){
		return false;
	}

	QStringList libRefs = PathManager::GetLibRef(srcPath);
	PathManager::Deduplication(libRefs);
	if (!libRefs.isEmpty()){
		for (QStringList::iterator ite = libRefs.begin(); ite != libRefs.end(); ite++)
		{
			QString tmpPath = destPath + ite->mid(ite->lastIndexOf("/"));
			if (!PathManager::CopyDir(*ite, tmpPath, true)){
				return false;
			}
			PathManager::CorrectLibRef(tmpPath);
		}
	}

	return true;
}

bool SrcPack::ReplacePakByTable(QString &path)
{
	for (QList<ReplacePakTable>::iterator ite = mpakTableList.begin(); ite != mpakTableList.end(); ite++)
	{
		switch (PathManager::ReplacePakInSrc(path, ite->GetSrcPakName().trimmed(), ite->GetDestPakName().trimmed()))
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

bool SrcPack::ReplaceAppPakByTable(QString &path)
{
	for (QList<ReplaceAppPakTable>::iterator ite = mappPakTableList.begin(); ite != mappPakTableList.end(); ite++)
	{
		switch (PathManager::ReplaceAppPakInSrc(path, ite->GetSrcPakName().trimmed(), ite->GetDestPakName().trimmed()))
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
		case 5:
			emit GenerateError(QStringLiteral("error:替换App包名出错,替换包名过程出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}
	}
	return true;
}

bool SrcPack::PrePack(QProcess &pprocess)
{
	QString mainProPath = mtmpSrcPath + "/" + mmainNm;


	if (!Pack::ReplaceStrByTable(mainProPath)){
		return false;
	}

	if (!Pack::ReplaceResByTable(mainProPath)){
		return false;
	}

	if (!ReplacePakByTable(mainProPath)){
		return false;
	}

	if (!ReplaceAppPakByTable(mainProPath)){
		return false;
	}

	QStringList libRefs = PathManager::GetLibRef(mainProPath);
	PathManager::Deduplication(libRefs);
	if (!libRefs.isEmpty()){
		for (QStringList::iterator ite = libRefs.begin(); ite != libRefs.end(); ite++)
		{
			if (!GenerateBuild(pprocess, *ite)){
				return false;
			}
		}
	}

	if (!GenerateBuild(pprocess, mainProPath)){
		return false;
	}

	return true;
}

bool SrcPack::GenerateBuild(QProcess &pprocess,QString &path)
{
	QString buildS = path + "/build.xml";
	QString antS = path + "/ant.properties";
	QFile buildF(buildS);
	QFile antF(antS);
	if (buildF.exists()){
		if (!buildF.remove()){
			emit GenerateError(QStringLiteral("删除原来旧build.xlm文件失败!"));
			return false;
		}
	}

	if (antF.exists()){
		if (!antF.remove()){
			emit GenerateError(QStringLiteral("删除原来旧ant.properties文件失败!"));
			return false;
		}
	}
	QString target = PathManager::GetTarget(path);
	if (target.isEmpty()){
		emit GenerateError(QStringLiteral("error:未在%1位置找到project.properties文件或者未指定target").arg(path));
		return false;
	}
	QString prepackBat = QStringLiteral("prepack.bat");
	QStringList param;
	target = target.remove(" ").remove("target=");
	param << "\"" + PathManager::GetAndroid() + "\"" << "\"" + PathManager::GetSdkToolsPath() + "\"" << target << "\"" + path + "\"";
	QString content = QStringLiteral("\nkey.store=%1\nkey.alias=%2\nkey.store.password=%3\nkey.alias.password=%4\n")
		.arg("../apk.keystore")
		.arg(PathManager::GetKeyAliases())
		.arg(PathManager::GetPasswd())
		.arg(PathManager::GetAliasesPasswd());
	PathManager::AppendContentToProperties(content, path);
	
	if (!Tools::ExecuteCmd(prepackBat, param, pprocess, PathManager::GetToolPath())){
		emit GenerateError(QStringLiteral("error:命令执行错误！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		return false;
	}
	return true;
}

bool SrcPack::PackFromSrc(QProcess &pprocess)
{
	QString srcpackBat = QStringLiteral("srcpack.bat");
	QStringList param;
	param << "\"" + PathManager::GetAnt() + "\"" << mtmpSrcPath + "/" + mmainNm;
	if (!Tools::ExecuteCmd(srcpackBat, param, pprocess, PathManager::GetToolPath())){
		emit GenerateError(QStringLiteral("error:命令执行错误！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		return false;
	}
	return true;
}