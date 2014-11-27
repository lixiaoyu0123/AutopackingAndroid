#ifndef DOWNLOAD_H
#define DOWNLOAD_H
#include <QtNetwork/QNetworkAccessManager>
#include <QFile>
#include <QEventLoop>

class DownLoad:public QObject
{
	Q_OBJECT
public:
	explicit DownLoad(QObject *parent = NULL,QString &url = QString(""));
	~DownLoad();
	bool Get(QByteArray &result);
	bool DownFile(QString &save);
protected:
private:
	QNetworkAccessManager mcore;
	QString murl;
	QString msave;
	QNetworkReply *mreply;
	QFile *mpfile;
	QEventLoop *mloop;
	bool mhasError;
	
private slots:
void WriteDataSlot(qint64 bytesReceived, qint64 bytesTotal);
void DownLoadFinishedSlot();
void DownErrorSlot();
};
#endif