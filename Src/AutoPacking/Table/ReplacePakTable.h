#ifndef REPLACEPAK_TABLE_H
#define REPLACEPAK_TABLE_H
#include <QString>

class ReplacePakTable
{
public:
	ReplacePakTable();
	void SetSrcPakName(QString &srcPakName);
	void SetDestPakName(QString &destPakName);
	QString GetSrcPakName();
	QString GetDestPakName();

	ReplacePakTable &operator=(const ReplacePakTable other);
protected:
private:
	QString msrcPakName;
	QString mdestPakName;
};
#endif