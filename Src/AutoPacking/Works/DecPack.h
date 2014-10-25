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
	virtual void Init(QString &inPath, QString &outPath, QString &channelId, QString &channelName, QString &channeltbID,
		QList<ReplaceStrTable> &strTableList, QList<ReplaceResTable> &resTableList, QList<ReplacePakTable> &pakTableList, QList<ReplaceAppPakTable> &appPakTableList, int taskId);
	virtual void Stop();
	virtual bool ReplacePakByTable();
	bool Unpacket(QString &inPath, QString &outPath,QProcess &pprocess);
	bool Dopacket(QString &inPath, QString &outPath, QProcess &pprocess);
	bool SignPacket(QString inPath, QString outPath, QProcess &pprocess);
	bool Zipalign(QProcess &pprocess);
	void CreatPath(QString &outPath, QString &channelId, QString &channelName, QString &channeltbId);
	void run();

private:
	QString mtmpUnpacketPath;
	QString minputPath;

};
#endif