#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include "DownLoad.h"

DownLoad::DownLoad(QObject *parent, QString &url) :QObject(parent),
mcore(parent),
murl(url),
msave(""),
mreply(NULL),
mpfile(NULL),
mloop(NULL),
mhasError(false)
{

}

DownLoad::~DownLoad()
{
	if (mreply != NULL){
		delete mreply;
		mreply = NULL;
	}
	if (mpfile != NULL){
		delete mpfile;
		mpfile = NULL;
	}
	if (mloop != NULL){
		mloop->deleteLater();
		mloop = NULL;
	}
}

bool DownLoad::Get(QByteArray &result)
{
	QUrl url(murl);
	QNetworkRequest *request = new QNetworkRequest(url);
	QNetworkReply *reply = mcore.get(*request);
	QEventLoop loop;
	connect(&mcore, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
	loop.exec();
	if (reply->error() == QNetworkReply::NoError){
		result = reply->readAll();
		delete reply;
		delete request;
		return true;
	}
	delete reply;
	delete request;
	return false;
}

bool DownLoad::DownFile(QString &save)
{
	msave = save;
	mhasError = false;
	mpfile = new QFile(msave);
	if (!mpfile->open(QIODevice::WriteOnly)){
		return false;
	}
	QUrl url(murl);
	QNetworkRequest *request = new QNetworkRequest(url);
	mreply = mcore.get(*request);
	connect(mreply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(WriteDataSlot(qint64, qint64)));
	connect(mreply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(DownErrorSlot()));
	connect(mreply, SIGNAL(finished()), this, SLOT(DownLoadFinishedSlot()));
	mloop = new QEventLoop();
	mloop->exec();
	if (mhasError){
		delete mreply;
		mreply = NULL;
		delete request;
		return false;
	}

	delete mreply;
	mreply = NULL;
	delete request;
	return true;
}

void DownLoad::DownErrorSlot()
{
	mhasError = true;
}

void DownLoad::DownLoadFinishedSlot()
{
	mpfile->flush();
	mpfile->close();
	delete mpfile;
	mpfile = NULL;
	mloop->exit();
	mloop->deleteLater();
	mloop = NULL;
}

void DownLoad::WriteDataSlot(qint64 bytesReceived, qint64 bytesTotal)
{
	mpfile->write(mreply->readAll());
}