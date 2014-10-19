#ifndef DO_PACK_H
#define DO_PACK_H
#include <QProcess>
#include "Table/ReplaceStrTable.h"
#include "Table/ReplaceResTable.h"
#include "Pack.h"

class DecPack :public Pack
{
	Q_OBJECT
public:
	explicit DecPack(QObject *parent = 0);
	virtual ~DecPack();
	virtual void Start(QString &inPath, QString &outPath, QString &channelId, QString &channelName, QString &channeltbID,
		QList<ReplaceStrTable> &strTableList, QList<ReplaceResTable> &resTableList, int taskId);
	virtual void Stop();
	void Unpacket(QString &inPath, QString &outPath);
	void Dopacket(QString &inPath, QString &outPath);
	void SignPacket(QString inPath, QString outPath);
	void Zipalign();
	void CreatPath(QString &outPath, QString &channelId, QString &channelName, QString &channeltbId);

private:
	QString mtmpUnpacketPath;


private slots :
	void UnpacketFinishedSlot(int stat);
	void DopacketFinishedSlot(int stat);
	void SignFinishedSlot(int stat);
	void ZipalignFinishedSlot(int stat);
};
#endif