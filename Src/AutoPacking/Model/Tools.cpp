#include <QTextCodec>
#include "Tools.h"

bool Tools::ExecuteCmd(QString exe, QStringList argument, QProcess &pprocess, QString workPath)
{
	QString enterPath = QStringLiteral("cd /d ") + "\"" + workPath + "\"" + QStringLiteral("\n");
	QString arg;
	arg.append(exe);
	for (QStringList::Iterator ite = argument.begin(); ite != argument.end(); ite++)
	{
		arg.append(" ");
		arg.append(*ite);
	}
	arg.append("\n");
	pprocess.start("cmd");
	pprocess.waitForStarted();
	QTextCodec *gbk = QTextCodec::codecForName("GBK");
	QByteArray byteEnterPath = gbk->fromUnicode(enterPath.constData(), enterPath.length());
	QByteArray byteCommand = gbk->fromUnicode(arg.constData(), arg.length());
	char *charEnterPath = byteEnterPath.data();
	char *charCommand = byteCommand.data();
	pprocess.write(byteEnterPath);
	pprocess.write(charCommand);
	pprocess.closeWriteChannel();
	if (!pprocess.waitForFinished(1000 * 60 * 20)){
		return false;
	}
	return true;
}