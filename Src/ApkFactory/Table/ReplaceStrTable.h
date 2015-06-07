#ifndef REPLACESTR_TABLE_H
#define REPLACESTR_TABLE_H
#include <QString>

class ReplaceStrTable
{
public:
	ReplaceStrTable();
	void SetFolder(QString &folder);
	void SetSrcStr(QString &srcStr);
	void SetDestStr(QString &destStr);
	QString GetFolder();
	QString GetSrcStr();
	QString GetDestStr();

	ReplaceStrTable &operator=(const ReplaceStrTable &other);

protected:
private:
	QString mfolder;
	QString msrcStr;
	QString mdestStr;
};
#endif