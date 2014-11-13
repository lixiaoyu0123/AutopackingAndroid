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
mloop(NULL)
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
	mpfile = new QFile(msave);
	if (!mpfile->open(QIODevice::WriteOnly)){
		return false;
	}
	QUrl url(murl);
	QNetworkRequest *request = new QNetworkRequest(url);
	mreply = mcore.get(*request);
	connect((QObject *)mreply, SIGNAL(readyRead()), this, SLOT(WriteDataSlot()));
	mloop = new QEventLoop();
	connect(mreply, SIGNAL(finished()), this, SLOT(DownLoadFinishedSlot()));
	mloop->exec();
	if (mreply->error() != QNetworkReply::NoError){
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

void DownLoad::WriteDataSlot()
{
	mpfile->write(mreply->readAll());
}