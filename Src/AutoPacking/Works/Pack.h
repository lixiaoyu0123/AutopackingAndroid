#ifndef PACK_H
#define PACK_H
#include <QThread>
#include <QProcess>
#include "Table/ReplaceStrTable.h"
#include "Table/ReplaceResTable.h"
#include "Table/ReplacePakTable.h"
#include "Table/ReplaceAppPakTable.h"

class Pack:public QThread
{
	Q_OBJECT
public:
	explicit Pack(QObject *parent = 0);
	virtual ~Pack() = 0;
	virtual void Init(QString &inPath, QString &outPath, QString &channelId, QString &channelName, QString &channeltbID,
		QList<ReplaceStrTable> &strTableList, QList<ReplaceResTable> &resTableList, QList<ReplacePakTable> &pakTableList, QList<ReplaceAppPakTable> &appPakTableList,int taskId) = 0;
	virtual void Stop();
	virtual int GetTaskId();
	virtual void KillTask();
protected:	
	virtual bool ReplaceStrByTable(QString &path);
	virtual bool ReplaceResByTable(QString &path);
	virtual bool ReplacePakByTable(QString &path) = 0;
	virtual bool ReplaceAppPakByTable(QString &path) = 0;
	virtual bool CheckError(QProcess &pprocess);


protected:
	QString moutFile;
	QString mtmpSignFile;
	QString mtmpPath;
	QString mchannelId;
	QString mchannelName;
	QString moutputPath;
	QString mchanneltbId;
	QProcess *mpprocess;
	int mtaskId;
	QList<ReplaceStrTable> mstrTableList;
	QList<ReplaceResTable> mresTableList;
	QList<ReplacePakTable> mpakTableList;
	QList<ReplaceAppPakTable> mappPakTableList;
private:
signals :
	void FinishSignal(int stat,int taskId);
	void GenerateError(QString errorLog);
};
#endif