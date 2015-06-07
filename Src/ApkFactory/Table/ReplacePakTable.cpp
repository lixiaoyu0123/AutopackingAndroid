#include "ReplacePakTable.h"

ReplacePakTable::ReplacePakTable()
{

}

void ReplacePakTable::SetSrcPakName(QString &srcPakName)
{
	msrcPakName = srcPakName;
}

void ReplacePakTable::SetDestPakName(QString &destPakName)
{
	mdestPakName = destPakName;
}

QString ReplacePakTable::GetSrcPakName()
{
	return msrcPakName;
}

QString ReplacePakTable::GetDestPakName()
{
	return mdestPakName;
}

ReplacePakTable &ReplacePakTable::operator = (const ReplacePakTable other)
{
	msrcPakName = other.msrcPakName;
	mdestPakName = other.mdestPakName;
	return *this;
}