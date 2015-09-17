#include <QDir>
#include <QTextCodec>
#include "Pack.h"
#include "Model/PathManager.h"

Pack::Pack(QObject *parent) :
QThread(parent),
moutFile(""),
mtmpSignFile(""),
mchannelId(""),
mchannelName(""),
mtaskId(0),
mpprocess(NULL)
{

}

Pack::~Pack()
{
	if (mpprocess != NULL){
		mpprocess->terminate();
		mpprocess->deleteLater();
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

void Pack::KillTask()
{
	if (mpprocess != NULL){
		mpprocess->close();
		mpprocess->deleteLater();
		mpprocess = NULL;
	}
	if (!PathManager::RemoveDir(mtmpPath)){
		emit GenerateError(QStringLiteral("error:清除缓存出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
	}
}

int Pack::GetTaskId()
{
	return mtaskId;
}

bool Pack::ReplaceStrByTable(QString &path)
{
	QString encode = PathManager::GetEncodeToStr();
	for (QList<ReplaceStrTable>::iterator ite = mstrTableList.begin(); ite != mstrTableList.end(); ite++)
	{
		QString findFile;
		QString file = ite->GetFolder().trimmed();
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

		switch (PathManager::ReplaceStrStrict(findFile, ite->GetSrcStr().trimmed(), ite->GetDestStr().trimmed(), encode))
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
		QString destAbsolutelyStr;
		QString srcStr;
		QString destStr;
		srcStr = ite->GetFolderSrc().trimmed();
		destStr = ite->GetFolderDest().trimmed();
		destStr.replace("\\", "/");
		if (srcStr.isEmpty() || destStr.isEmpty()){
			emit GenerateError(QStringLiteral("error:原资源文件夹或目标文件夹未配置！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}
		if (destStr.replace("\\", "/").startsWith("/")){
			destAbsolutelyStr = path + destStr;
		}
		else{
			destAbsolutelyStr = path + "/" + destStr;
		}

		QFileInfo finfoSrc(srcStr);
		QFileInfo finfoDest(destAbsolutelyStr);

		if (!finfoSrc.exists()){
			emit GenerateError(QStringLiteral("error:资源源文件不存在！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
			return false;
		}

		if (finfoSrc.isFile()){
			if (finfoDest.exists()){
				if (finfoDest.isFile()){
					if (!PathManager::CopyFile(srcStr, destAbsolutelyStr, true)){
						emit GenerateError(QStringLiteral("error:资源文件拷贝出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
						return false;
					}
				}
				else if (finfoDest.isDir()){
					QFileInfo srcFinfo(srcStr);
					QString destPath = destAbsolutelyStr + QString("/") + srcFinfo.fileName();
					if (!PathManager::CopyFile(srcStr, destPath, true)){
						emit GenerateError(QStringLiteral("error:资源文件拷贝出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
						return false;
					}
				}
				else{
					emit GenerateError(QStringLiteral("error:资源目标配置错误！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
					return false;
				}
			}
			else{
				QFileInfo srcFinfo(srcStr);
				QString destPath = destAbsolutelyStr + QString("/") + srcFinfo.fileName();
				QDir destDir = finfoDest.absoluteDir();
				if (!destDir.mkpath(finfoDest.absolutePath())){
					emit GenerateError(QStringLiteral("error:资源目录创建失败！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
					return false;
				}

				if (!PathManager::CopyFile(srcStr, destPath, true)){
					emit GenerateError(QStringLiteral("error:资源文件拷贝出错！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
					return false;
				}
			}
		}
		else if (finfoSrc.isDir()){
			if (finfoDest.exists() && finfoDest.isFile()){
				emit GenerateError(QStringLiteral("error:资源源为目录，目标配置不应该为文件！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
				return false;
			}
			else if (finfoDest.exists() && finfoDest.isDir()){
				QDir destDir = finfoDest.absoluteDir();
				if (!destDir.mkpath(finfoDest.absolutePath())){
					emit GenerateError(QStringLiteral("error:资源目录创建失败！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
					return false;
				}

				if (!PathManager::CopyDir(srcStr, destAbsolutelyStr, true)){
					emit GenerateError(QStringLiteral("error:资源目录拷贝失败！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
					return false;
				}
			}
			else{
				emit GenerateError(QStringLiteral("error:资源目标配置错误！渠道ID:%1,渠道名:%2\n").arg(mchannelId).arg(mchannelName));
				return false;
			}
		}
	}
	return true;
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