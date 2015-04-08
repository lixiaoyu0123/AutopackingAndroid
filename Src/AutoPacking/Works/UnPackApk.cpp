#include <QTextCodec>
#include "UnPackApk.h"
#include "Model/PathManager.h"

QMutex UnPackApk::MUTEX;
UnPackApk* UnPackApk::INSTANCE = NULL;
bool UnPackApk::ISWORKING = false;

UnPackApk *UnPackApk::GetInstance()
{
	MUTEX.lock();
	if (INSTANCE == NULL){
		INSTANCE = new UnPackApk();
	}
	MUTEX.unlock();
	return INSTANCE;
}

UnPackApk::UnPackApk(QObject *parent) :mpprocess(NULL), QThread(parent)
{
}

UnPackApk::~UnPackApk()
{
}

void UnPackApk::StartWork(QString &src, QString &dest)
{
	if (ISWORKING){
		return;
	}
	msrc = src;
	mdest = dest;
	QThread::start();
}

void UnPackApk::run()
{
	QString apkTool = PathManager::GetToolPath() + QStringLiteral("/apktool.bat");
	PathManager::RemoveDir(PathManager::GetDecTmpPath());
	QStringList params;
	params << QString("d") << QString("-f") << QString("-o") << "\"" + PathManager::GetDecTmpPath() + "\"" << "\"" + msrc.trimmed() + "\"";
	mpprocess = new QProcess(NULL);
	QString enterPath = QStringLiteral("cd /d ") + "\"" + PathManager::GetToolPath() + "\"" + QStringLiteral("\n");

	QString param = "\"" + apkTool + "\"";
	for (QStringList::iterator ite = params.begin(); ite != params.end(); ite++)
	{
		param.append(" ");
		param.append(*ite);
	}
	param.append("\n");

	QTextCodec *gbk = QTextCodec::codecForName("GBK");
	QByteArray byteParam = gbk->fromUnicode(param.constData(), param.length());
	QByteArray byteEnterPath = gbk->fromUnicode(enterPath.constData(), enterPath.length());
	mpprocess->start("cmd");
	mpprocess->waitForStarted();
	mpprocess->write(byteEnterPath);
	mpprocess->write(byteParam);
	mpprocess->closeWriteChannel();
	mpprocess->waitForFinished(1000 * 60 * 20);
	PathManager::CopyDir(PathManager::GetDecTmpPath(), mdest.trimmed(), true);
	PathManager::RemoveDir(PathManager::GetDecTmpPath());
	mpprocess->deleteLater();
	mpprocess = NULL;
	ISWORKING = false;
	emit FinishSignal();
}

void UnPackApk::Stop()
{
	QProcess killer;
	killer.start("taskkill", QStringList() << "/f" << "/im" << "java.exe");
	if (!killer.waitForStarted()){
		return;
	}
	if (!killer.waitForFinished()){
		return;
	}
}