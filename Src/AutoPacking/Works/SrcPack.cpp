#include <QDir>
#include "SrcPack.h"
#include "Model/PathManager.h"

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
	mpprocess = new QProcess(NULL);
	if (!CreatPath(moutputPath, mchannelId, mchannelName, mchanneltbId)){
		mpprocess->close();
		delete mpprocess;
		mpprocess = NULL;
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除上次缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}

	QString mainProPath = mtmpSrcPath + "/" + mmainNm;

	if (!CopySrc(PathManager::GetSrcPath(), mtmpSrcPath)){
		emit GenerateError(QStringLiteral("error:拷贝源码文件失败！渠道ID: %1, 渠道名 : %2\n").arg(mchannelId).arg(mchannelName));
		mpprocess->close();
		delete mpprocess;
		mpprocess = NULL;
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}

	QFile buildXml(mainProPath + QStringLiteral("/build.xml"));
	if (buildXml.exists() && !buildXml.remove()){
		emit GenerateError(QStringLiteral("error:删除原buld.xml文件失败！渠道ID: %1, 渠道名 : %2\n").arg(mchannelId).arg(mchannelName));
		mpprocess->close();
		delete mpprocess;
		mpprocess = NULL;
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}

	if (!PrePack(*mpprocess)){
		mpprocess->close();
		delete mpprocess;
		mpprocess = NULL;
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}

	if (!CheckError(*mpprocess)){
		mpprocess->close();
		delete mpprocess;
		mpprocess = NULL;
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}

	if (!PackFromSrc(*mpprocess)){
		mpprocess->close();
		delete mpprocess;
		mpprocess = NULL;
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}

	if (!CheckError(*mpprocess)){
		mpprocess->close();
		delete mpprocess;
		mpprocess = NULL;
		if (!PathManager::RemoveDir(mtmpPath)){
			emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		}
		emit FinishSignal(1, mtaskId);
		return;
	}

	QString apk = PathManager::GetReleaseApk(PathManager::GetBin(mainProPath));
	if (apk.isEmpty()){
		mpprocess->close();
		delete mpprocess;
		mpprocess = NULL;
		emit GenerateError(QStringLiteral("error:打包后的apk文件未找到！渠道ID: %1, 渠道名: %2\n").arg(mchannelId).arg(mchannelName));
		emit FinishSignal(1, mtaskId);
		return;
	}
	if (!PathManager::CopyFile(PathManager::GetBin(mainProPath) + QString("/") + apk, moutFile, true)){
		mpprocess->close();
		delete mpprocess;
		mpprocess = NULL;
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
	mpprocess->close();
	delete mpprocess;
	mpprocess = NULL;
	emit FinishSignal(0, mtaskId);
}

bool SrcPack::CreatPath(QString &outPath, QString &channelId, QString &channelName, QString &channeltbId)
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
	mtmpCodePath = srcPath;
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
	if (!PathManager::RemoveDir(desDir + "/bin")){
		return false;
	}

	QStringList libRefs = PathManager::GetLibRef(srcPath);
	if (!libRefs.isEmpty()){
		QDir dir;
		QString currentPath;
		QString srcDir;
		for (QStringList::iterator ite = libRefs.begin(); ite != libRefs.end(); ite++)
		{
			currentPath = ite->mid(ite->indexOf("=") + 1);
			currentPath.replace("\\\\", "/");
			int pos = currentPath.indexOf("/");
			if (pos > 0){
				pos = currentPath.indexOf("/", pos + 1);
				if (pos > 0){
					currentPath = currentPath.left(pos);
				}
			}
			dir.setPath(srcPath);
			if (!dir.cd(currentPath)){
				return false;
			}
			srcDir = dir.absolutePath();
			desDir = destPath + "/" + srcDir.mid(srcDir.lastIndexOf("/") + 1);
			if (!PathManager::CopyDir(dir.absolutePath(), desDir, true)){
				return false;
			}
		}
	}

	return true;
}

bool SrcPack::ReplacePakByTable(QString &path)
{
	for (QList<ReplacePakTable>::iterator ite = mpakTableList.begin(); ite != mpakTableList.end(); ite++)
	{
		switch (PathManager::ReplacePakInSrc(path, ite->GetSrcPakName(), ite->GetDestPakName()))
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
		switch (PathManager::ReplaceAppPakInSrc(path, ite->GetSrcPakName(), ite->GetDestPakName()))
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
	
	if (!GenerateBuild(pprocess, mainProPath)){
		return false;
	}

	//掌阅单本书在界面combox中索引为1
	if (PathManager::GetCustomWay() == 1){
		if (!ZySingle()){
			emit GenerateError(QStringLiteral("error:单本书制作出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}
	}

	QStringList libRefs = PathManager::GetLibRef(mainProPath);
	if (!libRefs.isEmpty()){
		QDir dir;
		QString currentPath;
		QString srcDir;
		for (QStringList::iterator ite = libRefs.begin(); ite != libRefs.end(); ite++)
		{
			currentPath = ite->mid(ite->indexOf("=") + 1);
			currentPath.replace("\\\\", "/");
			dir.setPath(mainProPath);
			if (!dir.cd(currentPath)){
				return false;
			}
			if (!GenerateBuild(pprocess, dir.absolutePath())){
				return false;
			}
		}
	}


	return true;
}

bool SrcPack::GenerateBuild(QProcess &pprocess,QString &path)
{
	QString buildS = path + "/build.xml";
	QFile buildF(buildS);
	if (buildF.exists()){
		if (!buildF.remove()){
			emit GenerateError(QStringLiteral("删除原来旧build.xlm文件失败!"));
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
		.arg(PathManager::GetKeyPath())
		.arg(PathManager::GetKeyAliases())
		.arg(PathManager::GetPasswd())
		.arg(PathManager::GetAliasesPasswd());
	PathManager::AppendContentToProperties(content, path);
	


	if (!ExecuteCmd(prepackBat, param, pprocess,PathManager::GetToolPath())){
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
	if (!ExecuteCmd(srcpackBat, param, pprocess,PathManager::GetToolPath())){
		emit GenerateError(QStringLiteral("error:命令执行错误！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
		return false;
	}
	return true;
}