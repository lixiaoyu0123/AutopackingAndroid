#ifndef ZYSINGLEBOOK_H
#define ZYSINGLEBOOK_H
#include <QObject>
#include <QString>

class ZySingleBook:public QObject
{
	Q_OBJECT
public:
	explicit ZySingleBook(QObject *parent = NULL,QString &bookId = QString(""),QString &save = QString(""));
	~ZySingleBook();
	bool MakeBook(bool &isMakeIcon);
protected:
private:
	bool GetCover();
	bool GetBookInfo();
	bool GetIcon();
private:

	QString mbookId;
	QString mnewBookId;
	QString msave;
	QString mcoverUrl;
	bool misMakeIcon;
};
#endif