#ifndef XLSXPARSER_H
#define XLSXPARSER_H
#include <QStringList>
#include "xlsxdocument.h"

class XlsxParser
{
public:
	XlsxParser(QString &fileName);
	~XlsxParser();
	QStringList GetAllSheet();
	bool SelectSheet(QString &sheet);
	int GetRows();
	int GetColumns();
	QVariant GetValue(int row, int column);
protected:
private:
	QXlsx::Document mxlsx;

};
#endif