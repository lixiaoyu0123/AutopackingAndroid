#include "Table/ReplaceStrTable.h"

ReplaceStrTable::ReplaceStrTable()
{

}

void ReplaceStrTable::SetFolder(QString &folder)
{
	mfolder = folder;
}

void ReplaceStrTable::SetSrcStr(QString &srcStr)
{
	msrcStr = srcStr;
}

void ReplaceStrTable::SetDestStr(QString &destStr)
{
	mdestStr = destStr;
}

QString ReplaceStrTable::GetFolder()
{
	return mfolder;
}

QString ReplaceStrTable::GetSrcStr()
{
	return msrcStr;
}

QString ReplaceStrTable::GetDestStr()
{
	return mdestStr;
}

ReplaceStrTable &ReplaceStrTable::operator = (const ReplaceStrTable &other)
{
	msrcStr = other.msrcStr;
	mdestStr = other.mdestStr;
	mfolder = other.mfolder;
	return *this;
}