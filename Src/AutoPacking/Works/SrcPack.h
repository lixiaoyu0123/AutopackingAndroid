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
	virtual void Start(QString &inPath, QString &outPath, QString &channelId, QString &channelName, QString &channeltbID,
		QList<ReplaceStrTable> &strTableList, QList<ReplaceResTable> &resTableList, int taskId);
	virtual void Stop();

	bool CopySrc(QString &srcPath,QString &destPath);
	void PrePack();
	void PackFromSrc();
	void CreatPath(QString &outPath, QString &channelId, QString &channelName, QString &channeltbId);
protected:
private:

	QString mtmpSrcPath;
private slots:
void PrePackFinishedSlot(int stat);
void PackFromSrcFinishedSlot(int stat);
};
#endif