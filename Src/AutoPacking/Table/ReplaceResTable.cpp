#include "Table/ReplaceResTable.h"

ReplaceResTable::ReplaceResTable()
{

}

void ReplaceResTable::SetFolderSrc(QString &folderSrc)
{
	mfolderSrc = folderSrc;
}

void ReplaceResTable::SetFolderDest(QString &folderDest)
{
	mfolderDest = folderDest;
}

QString ReplaceResTable::GetFolderSrc()
{
	return mfolderSrc;
}

QString ReplaceResTable::GetFolderDest()
{
	return mfolderDest;
}

ReplaceResTable &ReplaceResTable::operator = (const ReplaceResTable other)
{
	mfolderSrc = other.mfolderSrc;
	mfolderDest = other.mfolderDest;
	return *this;
}