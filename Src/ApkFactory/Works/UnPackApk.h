#ifndef UNPACK_APK_H
#define UNPACK_APK_H
#include <QThread>
#include <QProcess>
#include <QMutex>

class UnPackApk :public QThread
{
	Q_OBJECT
public:
	explicit UnPackApk(QObject *parent = 0);
	~UnPackApk();
	void StartWork(QString &src, QString &dest);
	void Stop();
static UnPackApk *GetInstance();
protected:
	void run();
private:
	QProcess *mpprocess;
	QString msrc;
	QString mdest;

	static QMutex MUTEX;
	static bool ISWORKING;
	static UnPackApk *INSTANCE;

signals:
	void FinishSignal();
};
#endif