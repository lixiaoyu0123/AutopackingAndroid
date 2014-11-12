#ifndef DOWNLOAD_H
#define DOWNLOAD_H
#include <QtNetwork/QNetworkAccessManager>

class DownLoad:public QObject
{
	Q_OBJECT
public:
	explicit DownLoad(QObject *parent = NULL);
	~DownLoad();
protected:
private:
	QNetworkAccessManager mcore;
private slots:

};
#endif