#ifndef SRC_PACK_H
#define SRC_PACK_H
#include <QProcess>
#include "Pack.h"

class SrcPack:public Pack
{
	Q_OBJECT
public:
	explicit SrcPack(QObject *parent = 0);
	virtual ~SrcPack();
	virtual void Init(QString &inPath, QString &outPath, QString &channelId, QString &channelName, QString &channeltbID,
		QList<ReplaceStrTable> &strTableList, QList<ReplaceResTable> &resTableList, QList<ReplacePakTable> &pakTableList, QList<ReplaceAppPakTable> &appPakTableList, int taskId);
	virtual void Stop();
	virtual bool ReplacePakByTable();
	bool CopySrc(QString &srcPath,QString &destPath);	
	bool PrePack(QProcess &pprocess);
	bool PackFromSrc(QProcess &pprocess);
	void CreatPath(QString &outPath, QString &channelId, QString &channelName, QString &channeltbId);
protected:
	void run();
private:

	QString mtmpSrcPath;
	QString moutputPath;
	QString mchanneltbId;
};
#endif