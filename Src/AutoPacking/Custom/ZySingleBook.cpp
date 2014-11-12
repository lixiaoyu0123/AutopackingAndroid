#include "ZySingleBook.h"

ZySingleBook::ZySingleBook(QString &bookId, QString &destination) :misGood(false),
mbookId(bookId),
mdestination(destination)
{
	GetCover();
}

ZySingleBook::~ZySingleBook()
{

}

void ZySingleBook::GetCover()
{

}