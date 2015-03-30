#ifndef TOOLS_H
#define TOOLS_H
#include <QString>
#include <QProcess>

class Tools
{
public:
	static bool ExecuteCmd(QString exe, QStringList argument, QProcess &pprocess, QString workPath);
protected:
private:
};
#endif