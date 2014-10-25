#ifndef REPLACEAPPPAK_TABLE_H
#define REPLACEAPPPAK_TABLE_H
#include <QString>

class ReplaceAppPakTable
{
public:
	ReplaceAppPakTable();
	void SetSrcPakName(QString &srcPakName);
	void SetDestPakName(QString &destPakName);
	QString GetSrcPakName();
	QString GetDestPakName();

	ReplaceAppPakTable &operator=(const ReplaceAppPakTable other);
protected:
private:
	QString msrcPakName;
	QString mdestPakName;
};
#endif