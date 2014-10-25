#include "ReplaceAppPakTable.h"

ReplaceAppPakTable::ReplaceAppPakTable()
{

}

void ReplaceAppPakTable::SetSrcPakName(QString &srcPakName)
{
	msrcPakName = srcPakName;
}

void ReplaceAppPakTable::SetDestPakName(QString &destPakName)
{
	mdestPakName = destPakName;
}

QString ReplaceAppPakTable::GetSrcPakName()
{
	return msrcPakName;
}

QString ReplaceAppPakTable::GetDestPakName()
{
	return mdestPakName;
}

ReplaceAppPakTable &ReplaceAppPakTable::operator = (const ReplaceAppPakTable other)
{
	msrcPakName = other.msrcPakName;
	mdestPakName = other.mdestPakName;
	return *this;
}