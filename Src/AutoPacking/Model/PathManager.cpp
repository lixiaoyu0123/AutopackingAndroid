#include <QDir>
#include <QVector>
#include <QSettings>
#include <QTextStream>
#include <QProcess>
#include <QRegularExpression>
#include <QtXml/QtXml>
#include <QtXml/QDomNodeList>
#include <QDebug>
#include "PathManager.h"
#include "Dialogs/BjMessageBox.h"
#include "PCRE/SPCRE.h"
#include "PCRE/PCRECache.h"


QString PathManager::OUTPATH = "";
QString PathManager::DECPACKPATH = "";
QString PathManager::KEYPATH = "";
QString PathManager::JDKPATH = "";
QString PathManager::SRCPATH = "";
QString PathManager::SDKPATH = "";
QString PathManager::PASSWD = "";
QString PathManager::ALIASESPASSWD = "";
QString PathManager::KEYALIASES = "";
QString PathManager::VERSION = "";
QString PathManager::SIGALG = "";
QString PathManager::DIGESTALG = "";
int PathManager::THREADNUM = 1;
int PathManager::PACKWAY = 0;
bool PathManager::BUILD_XML_WAY = false;

QString PathManager::GetStartPath()
{
	return QDir::currentPath();
}

QString PathManager::GetConfigPath()
{
	QString path("");
	if (!QStandardPaths::standardLocations(QStandardPaths::DataLocation).isEmpty()){
		path = QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation).first() + QStringLiteral("/Bingjian");
		QDir dir(path);
		if (!dir.exists(path)){
			dir.mkpath(path);
		}
	}
	return path;
}

QString PathManager::GetDataBasePath()
{
	QString path("");
	if (!GetConfigPath().isEmpty()){
		path = GetConfigPath() + "/database.db";
	}
	return path;
}

QString PathManager::GetToolPath()
{
	return GetStartPath() + "/Tools";
}

QString PathManager::GetDocumentsPath()
{
	return GetStartPath() + QStringLiteral("/Documents");
}

bool PathManager::AppendContentToProperties(QString &content, QString &path)
{
	QFile file(path + QStringLiteral("/project.properties"));
	if (!file.open(QIODevice::Append | QIODevice::Text))
		return false;
	QTextStream out(&file);
	out.setCodec("UTF-8");
	out << content;
	file.close();
	return true;
}

QString PathManager::GetTarget(QString &path)
{
	QFile file(path + QStringLiteral("/project.properties"));
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return QString("");

	QTextStream in(&file);
	in.setCodec("UTF-8");
	QString line;
	while (!in.atEnd()) {
		line = in.readLine();
		if (line.toLower().startsWith(QStringLiteral("target"))){
			file.close();
			return line;
		}
	}
	file.close();
	return QString("");
}

QStringList PathManager::GetLibRef(QString &path)
{
	QStringList retList;
	QFile file(path + QStringLiteral("/project.properties"));
	if (!file.exists()){
		return retList;
	}

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return retList;

	QTextStream in(&file);
	in.setCodec("UTF-8");
	QString line;
	while (!in.atEnd()) {
		line = in.readLine();
		if (line.toLower().startsWith(QStringLiteral("android.library.reference"))){
			QDir dir;
			QString currentPath;
			currentPath = line.mid(line.indexOf("=") + 1);
			currentPath.replace("\\\\", "/");
			dir.setPath(path);
			if (!dir.cd(currentPath)){
				continue;
			}
			currentPath = dir.absolutePath();
			retList.append(currentPath);
			QStringList refLibs = GetLibRef(currentPath);
			if (!refLibs.isEmpty()){
				retList.append(refLibs);
			}
		}
	}
	file.close();
	return retList;
}

void PathManager::Deduplication(QStringList &list)
{
	for (int i = 0; i < list.size(); i++)
	{
		for (int j = list.size() - 1; j > i; j--)
		{
			if (list[i] == list[j]){
				list.removeAt(i);
			}
		}
	}
}

void PathManager::CorrectLibRef(QString &path)
{
	QFile file(path + QStringLiteral("/project.properties"));
	if (!file.exists()){
		return;
	}

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	in.setCodec("UTF-8");
	QString line;
	QString content;
	while (!in.atEnd()) {
		line = in.readLine();
		if (line.toLower().startsWith(QStringLiteral("android.library.reference"))){
			QString currentPath;
			int index = line.indexOf("=");
			currentPath = line.mid(index + 1);
			currentPath.replace("\\\\", "/");
			line = line.left(index + 1) + "..\\\\" + currentPath.mid(currentPath.lastIndexOf("/") + 1);

		}
		content.append(line).append("\n");
	}

	file.close();
	if (!file.resize(0)){
		return;
	}

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
	out.setCodec("UTF-8");
	out << content;
	file.close();
}

QString PathManager::GetApkDir(QString &proFile)
{
	QString line;
	QFile antF(proFile + QStringLiteral("/ant.properties"));
	if (antF.exists()){
		if (antF.open(QIODevice::ReadOnly | QIODevice::Text)){

			QTextStream in(&antF);
			in.setCodec("UTF-8");
			while (!in.atEnd()) {
				line = in.readLine();
				if (line.toLower().startsWith(QStringLiteral("out.absolute.dir"))){
					line.remove(" ");
					line.replace("\\\\", "/");
					line = line.mid(line.indexOf("=") + 1);
					break;
				}
				else{
					line = "";
				}
			}
			antF.close();
		}
	}

	if (line.isEmpty()){
		QFile proF(proFile + QStringLiteral("/project.properties"));
		if (proF.open(QIODevice::ReadOnly | QIODevice::Text)){

			QTextStream in(&proF);
			in.setCodec("UTF-8");
			while (!in.atEnd()) {
				line = in.readLine();
				if (line.toLower().startsWith(QStringLiteral("out.absolute.dir"))){
					line.remove(" ");
					line.replace("\\\\", "/");
					line = line.mid(line.indexOf("=") + 1);
					break;
				}
				else{
					line = "";
				}
			}
			proF.close();
		}
	}
	return line;
}

QString PathManager::GetAntPath()
{
	return GetStartPath() + QStringLiteral("/apache-ant/bin");
}

QString PathManager::GetAnt()
{
	return GetAntPath() + QStringLiteral("/ant.bat");
}

QString PathManager::GetAndroid()
{
	return GetSdkToolsPath() + QStringLiteral("/android.bat");
}

QString PathManager::GetBin(QString &projectPath)
{
	return projectPath + QStringLiteral("/bin");
}

QString PathManager::GetReleaseApk(QString &path)
{
	QDir dir(path);
	QStringList files = dir.entryList(QDir::NoDotAndDotDot | QDir::Files);
	for (QStringList::iterator ite = files.begin(); ite != files.end(); ite++)
	{
		if (ite->toLower().endsWith("release.apk")){
			return *ite;
		}
	}
	return QString("");
}

QString PathManager::GetReleaseUnsignApk(QString &path)
{
	QDir dir(path);
	dir.setFilter(QDir::NoDotAndDotDot | QDir::Files);
	QStringList files = dir.entryList();
	for (QStringList::iterator ite = files.begin(); ite != files.end(); ite++)
	{
		if (ite->toLower().endsWith("unaligned.apk")){
			return *ite;
		}
	}
	return QString("");
}

QString PathManager::GetTmpPath()
{
	QString path("");
	if (!QStandardPaths::standardLocations(QStandardPaths::TempLocation).isEmpty()){
		path = QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation).first() + QStringLiteral("/Bingjian");
		QDir dir(path);
		if (!dir.exists(path)){
			dir.mkpath(path);
		}
	}
	return path;
}

QString PathManager::GetDecTmpPath()
{
	QString path = GetTmpPath() + "/DecTmp";
	QDir dir(path);
	if (!dir.exists()){
		dir.mkpath(path);
	}
	return path;
}

void PathManager::CreatDir(QString &dir)
{
	QDir dirTmp;
	if (!dirTmp.exists(dir)){
		dirTmp.mkpath(dir);
	}
}

void PathManager::SetOutPath(QString &path)
{
	OUTPATH = path;
	WriteSetting();
}

QString PathManager::GetOutPath()
{
	ReadSetting();
	return OUTPATH;
}

void PathManager::SetDecPackPath(QString &path)
{
	DECPACKPATH = path;
	WriteSetting();
}

QString PathManager::GetDecPackPath()
{
	ReadSetting();
	return DECPACKPATH;
}

void PathManager::SetKeyPath(QString &path)
{
	KEYPATH = path;
	WriteSetting();
}

QString PathManager::GetKeyPath()
{
	ReadSetting();
	return KEYPATH;
}

void PathManager::SetJdkPath(QString &path)
{
	JDKPATH = path;
	WriteSetting();
}

QString PathManager::GetJdkPath()
{
	ReadSetting();
	return JDKPATH;
}

void PathManager::SetSrcPath(QString &path)
{
	SRCPATH = path;
	WriteSetting();
}

QString PathManager::GetSrcPath()
{
	ReadSetting();
	return SRCPATH;
}

void PathManager::SetSdkPath(QString &path)
{
	SDKPATH = path;
	WriteSetting();
}

QString PathManager::GetSdkPath()
{
	ReadSetting();
	return SDKPATH;
}

QString PathManager::GetSdkToolsPath()
{
	if (GetSdkPath().isEmpty()){
		return "";
	}
	return GetSdkPath() + QStringLiteral("/tools");
}

void PathManager::SetPasswd(QString &password)
{
	PASSWD = password;
	WriteSetting();
}

QString PathManager::GetPasswd()
{
	ReadSetting();
	return PASSWD;
}

void PathManager::SetKeyAliases(QString &aliases)
{
	KEYALIASES = aliases;
	WriteSetting();
}

QString PathManager::GetKeyAliases()
{
	ReadSetting();
	return KEYALIASES;
}

void PathManager::SetAliasesPasswd(QString &password)
{
	ALIASESPASSWD = password;
	WriteSetting();
}

QString PathManager::GetAliasesPasswd()
{
	ReadSetting();
	return ALIASESPASSWD;
}

void PathManager::SetThreadNum(int num)
{
	THREADNUM = num;
	WriteSetting();
}

int PathManager::GetThreadNum()
{
	ReadSetting();
	return THREADNUM;
}

void PathManager::SetVersion(QString &ver)
{
	VERSION = ver;
	WriteSetting();
}

QString PathManager::GetVersion()
{
	ReadSetting();
	return VERSION;
}

QString PathManager::GetBuildXml()
{
	QString buildPath = GetSrcPath() + "/build.xml";
	return buildPath;
}

void PathManager::CheckBuildXml()
{
	QString buildXmlPath = PathManager::GetBuildXml();
	QFile buildF(buildXmlPath);
	if (buildF.exists()){
		BjMessageBox messageBox(NULL);
		messageBox.setWindowTitle(QStringLiteral("友情提示"));
		messageBox.setText(QStringLiteral("发现ant脚本文件build.xml，是否使用存在的脚本去打包？"));
		QPushButton *yes = messageBox.addButton((QStringLiteral("是")), QMessageBox::ActionRole);
		QPushButton *no = messageBox.addButton((QStringLiteral("否")), QMessageBox::RejectRole);
		// int ret = BjMessageBox::question(NULL, QStringLiteral("友情提示"), QStringLiteral("发现ant脚本文件build.xml，是否使用存在的脚本去打包？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::NoButton);
		messageBox.exec();
		if (messageBox.clickedButton() == (QAbstractButton *)yes){
			PathManager::SetBuildXmlWay(true);
		}
		else{
			PathManager::SetBuildXmlWay(false);
		}
	}
}

void PathManager::SetPackWay(int way)
{
	PACKWAY = way;
	WriteSetting();
}

int PathManager::GetPackWay()
{
	ReadSetting();
	return PACKWAY;
}

void PathManager::SetSigalg(QString &sig)
{
	SIGALG = sig;
	WriteSetting();
}

QString PathManager::GetSigalg()
{
	ReadSetting();
	return SIGALG;
}

void PathManager::SetDigestalg(QString &dig)
{
	DIGESTALG = dig;
	WriteSetting();
}

QString PathManager::GetDigestalg()
{
	ReadSetting();
	return DIGESTALG;
}

bool PathManager::RemoveDir(const QString &dirName)
{
	if (dirName.isEmpty()){
		return true;
	}
	QVector<QString> dirNames;
	QDir dir(dirName);
	QFileInfoList filst;
	QFileInfoList::iterator curFi;
	//初始化
	dirNames.clear();
	if (dir.exists()){
		dirNames << dirName;
	}
	else{
		return true;
	}
	//遍历各级文件夹，并将这些文件夹中的文件删除  
	for (int i = 0; i<dirNames.size(); ++i)
	{
		dir.setPath(dirNames[i]);
		filst = dir.entryInfoList(QDir::Dirs | QDir::Files
			| QDir::Readable | QDir::Writable
			| QDir::Hidden | QDir::NoDotAndDotDot
			, QDir::Name);
		if (filst.size()>0){
			curFi = filst.begin();
			while (curFi != filst.end())
			{
				//遇到文件夹,则添加至文件夹列表dirs尾部  
				if (curFi->isDir()){
					dirNames.push_back(curFi->filePath());
				}
				else if (curFi->isFile()){
					//遇到文件,则删除之  
					if (!dir.remove(curFi->absoluteFilePath())){
						return false;
					}
				}
				curFi++;
			}//end of while  
		}
	}
	//删除文件夹  
	for (int i = dirNames.size() - 1; i >= 0; --i)
	{
		dir.setPath(dirNames[i]);
		if (!dir.rmdir(".")){
			return false;
		}
	}
	return true;
}

bool PathManager::CopyDir(const QString &source, const QString &destination, bool isCover)
{
	/*  递归实现  */
	/*QDir directory(source);
	if (!directory.exists()){
	return false;
	}

	QString srcPath = QDir::toNativeSeparators(source);
	if (!srcPath.endsWith(QDir::separator())){
	srcPath += QDir::separator();
	}
	QString dstPath = QDir::toNativeSeparators(destination);
	if (!dstPath.endsWith(QDir::separator())){
	dstPath += QDir::separator();
	}

	QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
	for (QStringList::size_type i = 0; i != fileNames.size(); ++i)
	{
	QString fileName = fileNames.at(i);
	QString srcFilePath = srcPath + fileName;
	QString dstFilePath = dstPath + fileName;
	QFileInfo fileInfo(srcFilePath);
	if (fileInfo.isFile() || fileInfo.isSymLink()){
	if (isCover){
	QFile::setPermissions(dstFilePath, QFile::WriteOwner);
	}
	if (!QFile::copy(srcFilePath, dstFilePath)){
	return false;
	}
	}
	else if (fileInfo.isDir()){
	QDir dstDir(dstFilePath);
	dstDir.mkpath(dstFilePath);
	if (!CopyDir(srcFilePath, dstFilePath, isCover)){
	return false;
	}
	}
	}

	return true;*/

	QVector<QString> dirNames;
	QVector<QString> fileNames;
	QDir dir(source);
	if (!dir.exists()){
		return false;
	}
	dirNames.clear();
	dirNames << source;

	QFileInfoList filst;
	QFileInfoList::iterator curFi;

	for (int i = 0; i < dirNames.size(); ++i)
	{
		dir.setPath(dirNames[i]);
		filst = dir.entryInfoList(QDir::Dirs | QDir::Files
			| QDir::Readable | QDir::Writable
			| QDir::Hidden | QDir::NoDotAndDotDot
			, QDir::DirsFirst);
		if (filst.size() > 0){
			curFi = filst.begin();
			while (curFi != filst.end())
			{
				if (curFi->isDir()){
					dirNames.push_back(curFi->filePath());
				}
				else if (curFi->isFile()){
					fileNames.push_back(curFi->absoluteFilePath());
				}
				curFi++;
			}
		}
	}

	for (QVector<QString>::Iterator ite = dirNames.begin(); ite != dirNames.end(); ite++)
	{
		QDir dirSrc(source);
		QDir dirDest(destination);
		ite->replace(dirSrc.absolutePath(), dirDest.absolutePath());
		QDir dirTmp(*ite);
		if (!dirTmp.exists() && !dirTmp.mkpath(*ite)){
			return false;
		}
	}

	for (QVector<QString>::Iterator ite = fileNames.begin(); ite != fileNames.end(); ite++)
	{
		QDir dirSrc(source);
		QDir dirDest(destination);
		QString srcPath = *ite;

		QString tar = ite->replace(dirSrc.absolutePath(), dirDest.absolutePath());
		QFile file(tar);
		if (file.exists()){
			if (isCover){
				if (!file.remove()){
					return false;
				}
			}
			else{
				continue;
			}			
		}


		if (!QFile::copy(srcPath, tar)){
			return false;
		}
	}
	return true;
}

bool PathManager::SearchDirContianSuffix(const QString &dirFrom, QStringList &result, QString &suffix)
{
	QVector<QString> dirNames;
	QDir dir(dirFrom);
	QFileInfoList filst;
	QFileInfoList::iterator curFi;
	//初始化
	dirNames.clear();
	if (dir.exists()){
		dirNames << dirFrom;
	}
	else{
		return true;
	}
	//遍历各级文件夹，并将这些文件夹中的文件删除  
	for (int i = 0; i < dirNames.size(); ++i)
	{
		dir.setPath(dirNames[i]);
		filst = dir.entryInfoList(QDir::Dirs | QDir::Files
			| QDir::Readable | QDir::Writable
			| QDir::Hidden | QDir::NoDotAndDotDot
			, QDir::DirsFirst);
		if (filst.size()>0){
			curFi = filst.begin();
			while (curFi != filst.end())
			{
				//遇到文件夹,则添加至文件夹列表dirs尾部  
				if (curFi->isDir()){
					dirNames.push_back(curFi->filePath());
				}
				else if (curFi->isFile()){
					if (curFi->fileName().endsWith(suffix)){
						result.push_back(curFi->absolutePath());
						break;
					}
				}
				curFi++;
			}//end of while  
		}
	}
	return true;
}

void PathManager::RemoveEmptyDirFromDeepest(QString &path)
{
	QDir dir(path);
	QStringList allFilesDirs = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
	if (!allFilesDirs.isEmpty()){
		return;
	}
	dir.remove(path);
	QString newPath = path.left(path.lastIndexOf("/"));
	RemoveEmptyDirFromDeepest(newPath);
}

/**********
*返回值：0成功
*1原包名不存在
*2创建包出错
*3目的包名已经存在
*4替换包名过程出错
***********/
int PathManager::RenamePak(QString &path, QString &oldPakName, QString &newPakName)
{
	QString oldName = oldPakName;
	QString newName = newPakName;
	oldName.replace(".", "/");
	newName.replace(".", "/");

	QString srcDir = path + "/" + oldName;
	QString destDir = path + "/" + newName;
	QDir dirSrc(srcDir);
	QDir dirDest(destDir);
	if (!dirSrc.exists()){
		return 1;
	}

	dirSrc.setFilter(QDir::NoDotAndDotDot | QDir::Files);
	QFileInfoList allFiles = dirSrc.entryInfoList();
	if (allFiles.isEmpty()){
		return 1;
	}

	if (!dirDest.exists() && !dirDest.mkpath(destDir)){
		return 2;
	}
	dirDest.setFilter(QDir::NoDotAndDotDot | QDir::Files);
	if (!dirDest.entryInfoList().isEmpty()){
		return 3;
	}

	for (QFileInfoList::iterator ite = allFiles.begin(); ite != allFiles.end(); ite++){
		QString tmpName = ite->absoluteFilePath();
		tmpName = tmpName.mid(tmpName.lastIndexOf("/") + 1);
		if (!dirDest.exists()){
			if (dirDest.mkpath(destDir)){
				return 4;
			}
		}
		if (!CopyFile(ite->absoluteFilePath(), destDir + "/" + tmpName, true) || !QFile::remove(ite->absoluteFilePath())){
			return 4;
		}
	}

	RemoveEmptyDirFromDeepest(srcDir);
	return 0;
}

int PathManager::RenamePakInSrc(QString &path, QString &oldName, QString &newName)
{
	QString srcDir = path + QString("/src");
	return RenamePak(srcDir, oldName, newName);
}

int PathManager::RenamePakInDec(QString &path, QString &oldName, QString &newName)
{
	QString srcDir = path + QString("/smali");
	return RenamePak(srcDir, oldName, newName);
}

/**********
*返回值：0成功
*1原包名不存在
*2创建包出错
*3目的包名已经存在
*4替换包名过程出错
***********/
int PathManager::ReplacePakInSrc(QString &path, QString &oldName, QString &newName)
{
	QVector<QString> dirNames;
	QDir dir(path);
	QFileInfoList filst;
	QFileInfoList::iterator curFi;
	//初始化
	dirNames.clear();
	if (dir.exists()){
		dirNames << path;
	}
	else{
		return 0;
	}
	//遍历各级文件夹，并将这些文件夹中的文件删除  
	for (int i = 0; i<dirNames.size(); ++i)
	{
		dir.setPath(dirNames[i]);
		filst = dir.entryInfoList(QDir::Dirs | QDir::Files
			| QDir::Readable | QDir::Writable
			| QDir::Hidden | QDir::NoDotAndDotDot
			, QDir::Name);
		if (filst.size()>0){
			curFi = filst.begin();
			while (curFi != filst.end())
			{
				//遇到文件夹,则添加至文件夹列表dirs尾部  
				if (curFi->isDir()){
					dirNames.push_back(curFi->filePath());
				}
				else if (curFi->isFile()){
					//遇到文件,则删除之  
					if (curFi->absoluteFilePath().toLower().endsWith(".java")){
						if (!ReplacePakNameInJava(path, curFi->absoluteFilePath(), oldName, newName)){
							return 1;
						}
					}
					else if (curFi->absoluteFilePath().toLower().endsWith(".xml")){
						if (!ReplacePakNameInXml(path, curFi->absoluteFilePath(), oldName, newName)){
							return 1;
						}
					}
				}
				curFi++;
			}//end of while  
		}
	}

	int ret = 0;
	ret = RenamePakInSrc(path, oldName, newName);
	if (ret != 0){
		return ret;
	}
	return 0;
}

int PathManager::ReplacePakInDec(QString &path, QString &oldName, QString &newName)
{
	QVector<QString> dirNames;
	QDir dir(path);
	QFileInfoList filst;
	QFileInfoList::iterator curFi;
	//初始化
	dirNames.clear();
	if (dir.exists()){
		dirNames << path;
	}
	else{
		return 0;
	}
	//遍历各级文件夹，并将这些文件夹中的文件删除  
	for (int i = 0; i<dirNames.size(); ++i)
	{
		dir.setPath(dirNames[i]);
		filst = dir.entryInfoList(QDir::Dirs | QDir::Files
			| QDir::Readable | QDir::Writable
			| QDir::Hidden | QDir::NoDotAndDotDot
			, QDir::Name);
		if (filst.size()>0){
			curFi = filst.begin();
			while (curFi != filst.end())
			{
				//遇到文件夹,则添加至文件夹列表dirs尾部  
				if (curFi->isDir()){
					dirNames.push_back(curFi->filePath());
				}
				else if (curFi->isFile()){
					//遇到文件,则删除之  
					if (curFi->absoluteFilePath().toLower().endsWith(".smali")){
						if (!ReplacePakNameInSmali(curFi->absoluteFilePath(), oldName, newName)){
							return 1;
						}
					}
					else if (curFi->absoluteFilePath().toLower().endsWith(".xml")){
						if (!ReplacePakNameInXml(path, curFi->absoluteFilePath(), oldName, newName)){
							return 1;
						}
					}
				}
				curFi++;
			}//end of while  
		}
	}

	int ret = 0;
	ret = RenamePakInDec(path, oldName, newName);
	if (ret != 0){
		return ret;
	}
	return 0;
}

bool PathManager::ReplaceStr(QString &fileName, QString &srcStr, QString &replaceStr)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}
	QTextStream in(&file);
	in.setCodec("UTF-8");
	QString content;
	content = in.readAll();
	content.replace(srcStr, replaceStr);

	file.close();
	if (!QFile::remove(fileName)){
		return false;
	}
	QFile newfile(fileName);
	if (!newfile.open(QIODevice::ReadWrite | QIODevice::Text)){
		return false;
	}
	newfile.write(content.toUtf8());
	newfile.close();
	return true;
}

/******************************************************
*返回值：成功0
*		未发现原字符串1
*		原文件开错误2
*		未知错误3
******************************************************/
int PathManager::ReplaceStrStrict(QString &fileName, QString &srcStr, QString &replaceStr)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return 2;
	}
	QTextStream in(&file);
	in.setCodec("UTF-8");
	QString content;
	content = in.readAll();
	if (!content.contains(srcStr)){
		return 1;
	}
	content.replace(srcStr, replaceStr);

	file.close();
	if (!QFile::remove(fileName)){
		return 3;
	}
	QFile newfile(fileName);
	if (!newfile.open(QIODevice::ReadWrite | QIODevice::Text)){
		return 3;
	}
	newfile.write(content.toUtf8());
	newfile.close();
	return 0;
}

bool PathManager::ReplaceByRegular(QString &regular, QString &text, QString &replaceStr)
{
	SPCRE *spcre = PCRECache::instance()->getObject(regular);
	QList<SPCRE::MatchInfo> match_info = spcre->getEveryMatchInfo(text);
	for (int i = match_info.count() - 1; i >= 0; i--)
	{
		QString replaced_text;
		bool replacement_made = spcre->replaceText(text.mid(match_info.at(i).offset.first, match_info.at(i).offset.second - match_info.at(i).offset.first), match_info.at(i).capture_groups_offsets, replaceStr, replaced_text);
		if (replacement_made) {
			// Replace the text.
			text = text.replace(match_info.at(i).offset.first, match_info.at(i).offset.second - match_info.at(i).offset.first, replaced_text);
		}
	}
	return true;
}

bool PathManager::ReplacePakNameInXml(QString &srcPath, QString &fileName, QString &oldName, QString &newName)
{
	if (fileName.toLower().endsWith(".xml")){

		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
			return false;
		}
		QTextStream in(&file);
		in.setCodec("UTF-8");
		QString content;
		content = in.readAll();
		file.close();
		QString contentEx = QString("<[^<>]*%1[^<>]*>").arg(oldName);

		for (int i = 0; i < content.length();)
		{
			i = content.indexOf(QRegExp(contentEx), i);
			if (i < 0){
				break;
			}

			i = content.indexOf(oldName, i);
			if (i < 0){
				break;
			}
			int offset = i + oldName.length();
			if (offset >= content.length()){
				break;
			}
			QString remain = content.mid(i + oldName.length() + 1);
			int newOffset = remain.indexOf(QRegExp("\\.|\""), 0);
			if (newOffset < 0){
				break;
			}
			QString packName = oldName;
			QString className = remain.left(newOffset);
			QFile javaClass(srcPath + "/src/" + packName.replace(".", "/") + "/" + className + ".java");
			if (javaClass.exists()){
				content.remove(i, oldName.length());
				content.insert(i, newName);
			}
			i = offset;
		}

		if (!QFile::remove(fileName)){
			return false;
		}
		QFile newfile(fileName);
		if (!newfile.open(QIODevice::ReadWrite | QIODevice::Text)){
			return false;
		}
		newfile.write(content.toUtf8());
		newfile.close();
		return true;
	}
	return true;
}

bool PathManager::ReplaceAppPakNameInXml(QString &srcPath, QString &fileName, QString &oldName, QString &newName)
{
	QString oldNmSpace = "\"http://schemas.android.com/apk/res/" + oldName + "\"";
	QString newNmSpace = "\"http://schemas.android.com/apk/res/" + newName + "\"";
	if (!ReplaceStr(fileName, oldNmSpace, newNmSpace)){
		return false;
	}
	return true;
}

/**********
*返回值：0成功
*1原包名不存在
*2创建包出错
*3目的包名已经存在
*4替换包名过程出错
*5替换app包名出错
***********/
int PathManager::ReplaceAppPakInSrc(QString &path, QString &oldName, QString &newName)
{
	QVector<QString> dirNames;
	QDir dir(path);
	QFileInfoList filst;
	QFileInfoList::iterator curFi;
	//初始化
	dirNames.clear();
	if (dir.exists()){
		dirNames << path;
	}
	else{
		return 0;
	}

	bool isTopDir = true;
	//遍历各级文件夹，并将这些文件夹中的文件删除  
	for (int i = 0; i<dirNames.size(); ++i)
	{
		dir.setPath(dirNames[i]);
		filst = dir.entryInfoList(QDir::Dirs | QDir::Files
			| QDir::Readable | QDir::Writable
			| QDir::Hidden | QDir::NoDotAndDotDot
			, QDir::Name);
		if (isTopDir){
			isTopDir = false;
			for (int i = 0; i < filst.size(); i++)
			{
				if (filst[i].fileName() == "assets" || filst[i].fileName() == "bin" || filst[i].fileName() == "libs"){
					filst.removeAt(i);
					i--;
				}
			}
		}
		if (filst.size()>0){
			curFi = filst.begin();
			while (curFi != filst.end())
			{
				//遇到文件夹,则添加至文件夹列表dirs尾部  
				if (curFi->isDir()){
					dirNames.push_back(curFi->filePath());
				}
				else if (curFi->isFile()){
					if (curFi->absoluteFilePath().toLower().endsWith(".java")){
						if (!ReplaceAppPakNameInJava(path, curFi->absoluteFilePath(), oldName, newName)){
							return 1;
						}
					}
					else if (curFi->absoluteFilePath().toLower().endsWith(".xml")){
						if (!ReplaceAppPakNameInXml(path, curFi->absoluteFilePath(), oldName, newName)){
							return 1;
						}
					}
				}
				curFi++;
			}//end of while  
		}
	}

	if (!ReplaceAppPakNameInManifest(path, oldName, newName)){
		return 5;
	}

	return 0;
}

/**********
*返回值：0成功
*1原包名不存在
*2创建包出错
*3目的包名已经存在
*4替换包名过程出错
*5替换app包名出错
***********/
int PathManager::ReplaceAppPakInDec(QString &path, QString &oldName, QString &newName)
{
	QVector<QString> dirNames;
	QDir dir(path);
	QFileInfoList filst;
	QFileInfoList::iterator curFi;
	//初始化
	dirNames.clear();
	if (dir.exists()){
		dirNames << path;
	}
	else{
		return 0;
	}
	//遍历各级文件夹，并将这些文件夹中的文件删除  
	for (int i = 0; i<dirNames.size(); ++i)
	{
		dir.setPath(dirNames[i]);
		filst = dir.entryInfoList(QDir::Dirs | QDir::Files
			| QDir::Readable | QDir::Writable
			| QDir::Hidden | QDir::NoDotAndDotDot
			, QDir::Name);
		if (filst.size()>0){
			curFi = filst.begin();
			while (curFi != filst.end())
			{
				//遇到文件夹,则添加至文件夹列表dirs尾部  
				if (curFi->isDir()){
					dirNames.push_back(curFi->filePath());
				}
				else if (curFi->isFile()){
					//遇到文件,则删除之  
					if (curFi->absoluteFilePath().toLower().endsWith(".smali")){
						if (!ReplaceAppPakNameInSmali(curFi->absoluteFilePath(), oldName, newName)){
							return 1;
						}
					}
					else if (curFi->absoluteFilePath().toLower().endsWith(".xml")){
						if (!ReplaceAppPakNameInXml(path, curFi->absoluteFilePath(), oldName, newName)){
							return 1;
						}
					}
				}
				curFi++;
			}//end of while  
		}
	}

	if (!ReplaceAppPakNameInManifest(path, oldName, newName)){
		return 5;
	}

	return 0;
}

bool PathManager::ReplaceAppPakNameInManifest(QString &path, QString &oldName, QString &newName)
{
	QString manifest = path + "/" + "AndroidManifest.xml";
	QFile file(manifest);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}
	QDomDocument doc;
	if (!doc.setContent(&file)){
		file.close();
		return false;
	}
	file.close();
	QDomElement root = doc.documentElement();
	if (root.tagName() != "manifest"){
		return false;
	}
	QString pack = root.attribute("package");
	if (pack.isEmpty()){
		return false;
	}
	root.setAttribute("package", newName);

	QList<QDomElement> allNode;
	if (!root.firstChildElement().isNull()){
		QList<QDomElement> traversal;
		traversal.push_back(root.firstChildElement());
		for (int i = 0; i < traversal.size(); i++)
		{
			QDomElement indexNode = traversal.at(i);
			while (!indexNode.isNull())
			{
				if (indexNode.hasChildNodes()){
					traversal.push_back(indexNode.firstChildElement());
				}
				allNode.push_back(indexNode);

				indexNode = indexNode.nextSiblingElement();
			}

		}
	}

	for (QList<QDomElement>::Iterator ite = allNode.begin(); ite != allNode.end(); ite++)
	{
		QDomElement element = *ite;
		if (!element.isNull()){
			QString androidNm = element.attribute("android:name", "");
			androidNm = androidNm.toLower();
			QString fileStr = androidNm;
			fileStr.replace(".", "/");
			fileStr = path + "/src/" + fileStr + ".java";
			QFile fileClass(fileStr);
			if (androidNm.contains(oldName) && !fileClass.exists()){
				androidNm.replace(oldName, newName);
				element.setAttribute("android:name", androidNm);
			}
		}
	}

	QFile filexml(manifest);
	if (!filexml.open(QFile::WriteOnly | QFile::Truncate)){
		qWarning("error::ParserXML->writeOperateXml->file.open\n");
		return false;
	}
	QTextStream ts(&filexml);
	ts.reset();
	ts.setCodec("utf-8");
	doc.save(ts, 4, QDomNode::EncodingFromTextStream);
	filexml.close();
	return true;
}

bool PathManager::ReplacePakNameInSmali(QString &fileName, QString &oldName, QString &newName)
{
	if (fileName.toLower().endsWith(".smali")){
		QString oldNameTmp = oldName;
		QString newNameTmp = newName;
		oldNameTmp.replace(".", "/");
		newNameTmp.replace(".", "/");
		return ReplaceStr(fileName, oldNameTmp, newNameTmp);
	}
	return true;
}

bool PathManager::ReplaceAppPakNameInSmali(QString &fileName, QString &oldName, QString &newName)
{
	if (fileName.toLower().endsWith(".smali")){
		QString oldNameTmp = oldName;
		QString newNameTmp = newName;
		oldNameTmp.replace(".", "/");
		newNameTmp.replace(".", "/");
		return ReplaceStr(fileName, oldNameTmp, newNameTmp);
	}
	return true;
}

bool PathManager::ReplacePakNameInJava(QString &srcPath, QString &fileName, QString &oldName, QString &newName)
{
	if (fileName.toLower().endsWith(".java")){
		QString regularEx = QString("(?<=(\\bpackage)|(\\bimport))[\\s\\t\\r\\n]+%1[\\s\\t\\r\\n]*;").arg(oldName);
		QString replaceName = QString(" ") + newName + ";";
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
			return false;
		}
		QTextStream in(&file);
		in.setCodec("UTF-8");
		QString content;
		content = in.readAll();
		file.close();
		ReplaceByRegular(regularEx, content, replaceName);
		QString contentEx = QString("(?<=[({};])[\\s\\t\\r\\n]*%1\\.").arg(oldName);
		SPCRE *spcre = PCRECache::instance()->getObject(contentEx);
		QList<SPCRE::MatchInfo> match_info = spcre->getEveryMatchInfo(content);
		for (int i = match_info.count() - 1; i >= 0; i--)
		{
			QString replaced_text;
			bool replacement_made = spcre->replaceText(content.mid(match_info.at(i).offset.first, match_info.at(i).offset.second - match_info.at(i).offset.first), match_info.at(i).capture_groups_offsets, newName + ".", replaced_text);
			if (replacement_made) {
				int pos = content.indexOf(QRegExp("\\.|\\s"), match_info.at(i).offset.second);
				// Replace the text.
				if (pos < 0){
					continue;
				}
				QString classNm = content.mid(match_info.at(i).offset.second, pos - match_info.at(i).offset.second);
				QString packName = oldName;
				QFile javaClass(srcPath + "/src/" + packName.replace(".", "/") + "/" + classNm + ".java");
				if (javaClass.exists()){
					content = content.replace(match_info.at(i).offset.first, match_info.at(i).offset.second - match_info.at(i).offset.first, replaced_text);
				}
			}
		}

		if (!QFile::remove(fileName)){
			return false;
		}
		QFile newfile(fileName);
		if (!newfile.open(QIODevice::ReadWrite | QIODevice::Text)){
			return false;
		}
		newfile.write(content.toUtf8());
		newfile.close();
		return true;
	}
	return true;
}

bool PathManager::ReplaceAppPakNameInJava(QString &srcPath, QString &fileName, QString &oldName, QString &newName)
{
	if (fileName.toLower().endsWith(".java")){
		QString regularEx = QString("(?<=(\\bpackage)|(\\bimport))[\\s\\t\\r\\n]+%1[\\s\\t\\r\\n]*;").arg(oldName);
		QString replaceName = QString(" ") + newName + ";";
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
			return false;
		}
		QTextStream in(&file);
		in.setCodec("UTF-8");
		QString content;
		content = in.readAll();
		file.close();
		ReplaceByRegular(regularEx, content, replaceName);
		QString regularExR = QString("(?<=(\\bpackage)|(\\bimport))[\\s\\t\\r\\n]+%1%2[\\s\\t\\r\\n]*;").arg(oldName).arg(".R");
		QString replaceNameR = QString(" ") + newName + ".R;";
		ReplaceByRegular(regularExR, content, replaceNameR);
		content.replace(" " + oldName + ".R.", " " + newName + ".R.");
		if (!QFile::remove(fileName)){
			return false;
		}
		QFile newfile(fileName);
		if (!newfile.open(QIODevice::ReadWrite | QIODevice::Text)){
			return false;
		}
		newfile.write(content.toUtf8());
		newfile.close();
		return true;
	}
	return true;
}

bool PathManager::RewriteApktoolYml(QString &fileName)
{
	QFile file(fileName + QStringLiteral("/apktool.yml"));
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QTextStream in(&file);
	in.setCodec("UTF-8");
	QString content;
	QString line;
	while (!in.atEnd()) {
		line.clear();
		line = in.readLine();
		if (!line.toLower().startsWith(QStringLiteral("apkfilename:"))){
			content.append(line).append("\n");
			continue;
		}
		//apkFileName: new.apk 中间空格非常重要，非有不可
		content.append("apkFileName: new.apk").append("\n");
	}
	file.close();
	if (!file.resize(0)){
		return false;
	}

	if (!file.open(QIODevice::WriteOnly)){
		return false;
	}
	file.write(content.toUtf8());
	file.close();
	return true;
}

bool PathManager::CopyFile(const QString &SrcFile, const QString &DestFile, bool isCover)
{
	if (!QFile::exists(SrcFile)){
		return true;
	}

	if (QFile::exists(DestFile)){
		if (isCover){
			if (!QFile::remove(DestFile)){
				return false;
			}
		}
		else{
			return false;
		}
	}
	QFile oldFile(SrcFile);
	return oldFile.copy(DestFile);
}

bool PathManager::CheckSysEnvironment()
{
	QString javaHome = QString("");
	QStringList environment = QProcess::systemEnvironment();

	for (QStringList::iterator ite = environment.begin(); ite != environment.end(); ite++)
	{
		if (ite->toLower().startsWith("path=")){
			QStringList pathValueList = ite->mid(QString("path=").length()).split(";");
			for (QStringList::iterator ite = pathValueList.begin(); ite != pathValueList.end(); ite++)
			{
				QString path = ite->toLower();
				if (path.contains("java") && path.contains("bin")){
					SetJdkPath(ite->replace("\\", "/"));
					break;
				}
			}
			continue;
		}
		else if (ite->toLower().startsWith("java_home")){
			javaHome = *ite;
		}
	}

	if (JDKPATH.isEmpty()){
		BjMessageBox::warning(NULL, QStringLiteral("jdk环境变量错误"), QStringLiteral("在系统环境变量中未发现jdk路径，请先设置jdk环境变量，如果已经设置jdk环境变量，请在设置中指定其位置！"), QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	else if (javaHome.isEmpty()){
		BjMessageBox::warning(NULL, QStringLiteral("java_home环境变量错误"), QStringLiteral("在系统环境变量中未发现java_home环境变量，请先设置java_home环境变量！"), QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	return true;
}

bool PathManager::CheckParameter()
{
	QString parameter = QString("");
	if (OUTPATH.isEmpty()){
		parameter.append(QStringLiteral("输出路径为空\n"));
		BjMessageBox::warning(NULL, QStringLiteral("参数错误！"), parameter, QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	else if (KEYPATH.isEmpty() || !QFile(KEYPATH).exists()){
		parameter.append(QStringLiteral("密钥位置未设置或指定位置密钥不存在\n"));
		BjMessageBox::warning(NULL, QStringLiteral("参数错误！"), parameter, QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	else if (JDKPATH.isEmpty()){
		parameter.append(QStringLiteral("jdk位置未设置\n"));
		BjMessageBox::warning(NULL, QStringLiteral("参数错误！"), parameter, QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	else if (KEYALIASES.isEmpty()){
		parameter.append(QStringLiteral("密钥别名未设置\n"));
		BjMessageBox::warning(NULL, QStringLiteral("参数错误！"), parameter, QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	else if (ALIASESPASSWD.isEmpty()){
		parameter.append(QStringLiteral("别名密码未设置\n"));
		BjMessageBox::warning(NULL, QStringLiteral("参数错误！"), parameter, QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	else if (PASSWD.isEmpty()){
		parameter.append(QStringLiteral("签名密码未设置\n"));
		BjMessageBox::warning(NULL, QStringLiteral("参数错误！"), parameter, QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	else if (SIGALG.isEmpty()){
		parameter.append(QStringLiteral("签名算法未设置\n"));
		BjMessageBox::warning(NULL, QStringLiteral("参数错误！"), parameter, QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	else if (DIGESTALG.isEmpty()){
		parameter.append(QStringLiteral("摘要算法未设置\n"));
		BjMessageBox::warning(NULL, QStringLiteral("参数错误！"), parameter, QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	return true;
}

bool PathManager::IsFirstRun()
{
	QSettings settings(GetConfigPath() + QStringLiteral("/Config.ini"), QSettings::IniFormat);
	settings.beginGroup("Setting");
	QString firstRun = settings.value("FirstRun", "").toString();
	if (firstRun.isEmpty()){
		settings.setValue("FirstRun", QString("false"));
		settings.endGroup();
		return true;
	}
	settings.endGroup();
	return false;
}

void PathManager::SetBuildXmlWay(bool isUseDef)
{
	BUILD_XML_WAY = isUseDef;
}

//true为使用原来的buildxml文件 false为不使用原来的buildxml文件
bool PathManager::GetBuilXmlWay()
{
	return BUILD_XML_WAY;
}

void PathManager::WriteSetting()
{
	QSettings settings(GetConfigPath() + QStringLiteral("/Config.ini"), QSettings::IniFormat);
	settings.beginGroup("Setting");
	settings.setValue("OutPath", OUTPATH);
	settings.setValue("DecPackPath", DECPACKPATH);
	settings.setValue("KeyPath", KEYPATH);
	settings.setValue("JdkPath", JDKPATH);
	settings.setValue("SrcPath", SRCPATH);
	settings.setValue("SdkPath", SDKPATH);
	settings.setValue("PassWord", PASSWD);
	settings.setValue("KeyAliases", KEYALIASES);
	settings.setValue("AliasesPasswd", ALIASESPASSWD);
	settings.setValue("Version", VERSION);
	settings.setValue("ThreadNum", THREADNUM);
	settings.setValue("PackWay", PACKWAY);
	settings.setValue("Sigalg", SIGALG);
	settings.setValue("Digestalg", DIGESTALG);
	settings.endGroup();
}

void PathManager::ReadSetting()
{
	QSettings settings(GetConfigPath() + QStringLiteral("/Config.ini"), QSettings::IniFormat);
	settings.beginGroup("Setting");
	OUTPATH = settings.value("OutPath", "").toString();
	DECPACKPATH = settings.value("DecPackPath", "").toString();
	KEYPATH = settings.value("KeyPath", "").toString();
	JDKPATH = settings.value("JdkPath", "").toString();
	SRCPATH = settings.value("SrcPath", "").toString();
	SDKPATH = settings.value("SdkPath", "").toString();
	PASSWD = settings.value("PassWord", "").toString();
	KEYALIASES = settings.value("KeyAliases", "").toString();
	ALIASESPASSWD = settings.value("AliasesPasswd", "").toString();
	VERSION = settings.value("Version", "").toString();
	SIGALG = settings.value("Sigalg", "").toString();
	DIGESTALG = settings.value("Digestalg", "").toString();
	THREADNUM = settings.value("ThreadNum", "1").toInt();
	PACKWAY = settings.value("PackWay", "1").toInt();
	settings.endGroup();
}

void PathManager::WriteLastPath(QString &key, QString &val)
{
	QSettings settings(GetConfigPath() + QStringLiteral("/Config.ini"), QSettings::IniFormat);
	settings.beginGroup("LastPathSetting");
	settings.setValue(key, val);
	settings.endGroup();
}

QString PathManager::ReadLastPath(QString &key)
{
	QString ret;
	QSettings settings(GetConfigPath() + QStringLiteral("/Config.ini"), QSettings::IniFormat);
	settings.beginGroup("LastPathSetting");
	ret = settings.value(key, "").toString();
	settings.endGroup();
	return ret;
}