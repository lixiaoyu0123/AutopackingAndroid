#ifndef ZYSINGLEBOOK_H
#define ZYSINGLEBOOK_H
#include <QString>

class ZySingleBook
{
public:
	explicit ZySingleBook(QString &bookId,QString &destination);
	~ZySingleBook();
protected:
private:
	void GetCover();
private:
	bool misGood;
	QString mbookId;
	QString mdestination;
};
#endif