#ifndef REPLACERES_TABLE_H
#define REPLACERES_TABLE_H
#include <QString>

class ReplaceResTable
{
public:
	ReplaceResTable();
	void SetFolderSrc(QString &folderSrc);
	void SetFolderDest(QString &folderDest);
	QString GetFolderSrc();
	QString GetFolderDest();

	ReplaceResTable &operator=(const ReplaceResTable other);
protected:
private:
	QString mfolderSrc;
	QString mfolderDest;
};
#endif