#include "XlsxParser.h"

XlsxParser::XlsxParser(QString &fileName):mxlsx(fileName)
{
}

XlsxParser::~XlsxParser()
{

}

QStringList XlsxParser::GetAllSheet()
{
	return mxlsx.sheetNames();
}

int XlsxParser::GetRows()
{
	return mxlsx.dimension().rowCount();
}

int XlsxParser::GetColumns()
{
	return mxlsx.dimension().columnCount();
}

QVariant XlsxParser::GetValue(int row, int column)
{
	return mxlsx.cellAt(row, column)->value();
}

bool XlsxParser::SelectSheet(QString &sheet)
{
	return mxlsx.selectSheet(sheet);
}