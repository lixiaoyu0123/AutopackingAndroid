#ifndef PACK_H
#define PACK_H
#include <QObject>
#include <QProcess>
#include "Table/ReplaceStrTable.h"
#include "Table/ReplaceResTable.h"
#include "Table/ReplacePakTable.h"

class Pack:public QObject
{
	Q_OBJECT
public:
	explicit Pack(QObject *parent = 0);
	virtual ~Pack() = 0;
	virtual void Start(QString &inPath, QString &outPath, QString &channelId, QString &channelName, QString &channeltbID,
		QList<ReplaceStrTable> &strTableList, QList<ReplaceResTable> &resTableList, QList<ReplacePakTable> &pakTableList,int taskId) = 0;
	virtual void Stop() = 0;
	virtual int GetTaskId();
protected:	
	virtual bool ReplaceStrByTable(QString &path);
	virtual bool ReplaceResByTable(QString &path);
	virtual bool ReplacePakByTable() = 0;
	virtual bool CheckError();
	virtual void ExecuteCmd(QString exe, QStringList argument, QString workPath = QString(""));

protected:
	QProcess *mpprocess;
	QString moutFile;
	QString mtmpSignFile;
	QString mtmpPath;
	QString mchannelId;
	QString mchannelName;
	int mtaskId;
	QList<ReplaceStrTable> mstrTableList;
	QList<ReplaceResTable> mresTableList;
	QList<ReplacePakTable> mpakTableList;
private:
signals :
	void FinishSignal(int stat,int taskId);
	void GenerateError(QString errorLog);
};
#endif