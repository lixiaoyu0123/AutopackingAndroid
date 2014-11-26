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
	virtual bool ReplacePakByTable(QString &path);
	virtual bool ReplaceAppPakByTable(QString &path);
	bool CopySrc(QString &srcPath,QString &destPath);	
	bool PrePack(QProcess &pprocess);
	bool PackFromSrc(QProcess &pprocess);
	bool CreatPath(QString &outPath, QString &channelId, QString &channelName, QString &channeltbId);
	bool GenerateBuild(QProcess &pprocess ,QString &path);
protected:
	void run();
private:

	QString mtmpSrcPath;
	QString moutputPath;
	QString mchanneltbId;
	QString mmainNm;
};
#endif