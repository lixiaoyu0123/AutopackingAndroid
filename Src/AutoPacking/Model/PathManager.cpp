#include <QDir>
#include <QVector>
#include <QSettings>
#include <QTextStream>
#include <QProcess>
#include <QDebug>
#include "PathManager.h"
#include "Dialogs/BjMessageBox.h"


QString PathManager::OUTPATH = "";
QString PathManager::DECPACKPATH = "";
QString PathManager::KEYPATH = "";
QString PathManager::JDKPATH = "";
QString PathManager::SRCPATH = "";
QString PathManager::SDKPATH = "";
QString PathManager::PASSWD = "";
QString PathManager::KEYALIASES = "";
QString PathManager::VERSION = "";
QString PathManager::SIGALG = "";
QString PathManager::DIGESTALG = "";
int PathManager::THREADNUM = 1;
int PathManager::PACKWAY = 0;

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

QString PathManager::GetApkToolPath()
{
	return GetToolPath() + QStringLiteral("/apktool.bat");
}

QString PathManager::GetSrcPack()
{
	return GetToolPath() + QStringLiteral("/srcpack.bat");
}

QString PathManager::GetPrePack()
{
	return GetToolPath() + QStringLiteral("/prepack.bat");
}

bool PathManager::AppendContentToProperties(QString &content,QString &path)
{
	QFile file(path + QStringLiteral("/project.properties"));
	if (!file.open(QIODevice::Append | QIODevice::Text))
		return false;
	QTextStream out(&file);
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
	while (!in.atEnd()) {
		QString line = in.readLine();
		if (line.startsWith(QStringLiteral("target"))){
			file.close();
			return line;
		}
	}
	file.close();
	return QString("");
}

QString PathManager::GetZipalign()
{
	return GetToolPath() + QStringLiteral("/zipalign.exe");
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
	dir.setFilter(QDir::NoDotAndDotDot|QDir::Files);
	QStringList files = dir.entryList();
	for (QStringList::iterator ite = files.begin(); ite != files.end(); ite++)
	{
		if (ite->endsWith("release.apk")){
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
		if (ite->endsWith("unaligned.apk")){
			return *ite;
		}		
	}
	return QString("");
}

QString PathManager::GetJarSigner()
{
	return GetJdkPath() + QStringLiteral("/jarsigner.exe");
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
	password;
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
					if (!dir.remove(curFi->fileName())){
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
		if (!dirTmp.mkpath(*ite)){
			return false;
		}
	}

	for (QVector<QString>::Iterator ite = fileNames.begin(); ite != fileNames.end(); ite++)
	{
		if (isCover){
			QFile::setPermissions(*ite, QFile::WriteOwner);
		}
		QDir dirSrc(source);
		QDir dirDest(destination);
		QString srcPath = *ite;
		if (!QFile::copy(srcPath, ite->replace(dirSrc.absolutePath(), dirDest.absolutePath()))){
			return false;
		}
	}
	return true;
}

bool PathManager::RenamePak(QString &path, QString &oldName, QString &newName)
{
	QString oldNameTmp = oldName;
	QString newNameTmp = newName;
	QString srcDir = path + QString("/src");
	QString oldPath = srcDir + QString("/") + oldNameTmp.replace(".", "/");
	QString newPath = srcDir + QString("/") + newName.replace(".", "/");
	if (!CopyDir(oldPath, newPath, true)){
		return false;
	}
	if (!RemoveDir(oldPath)){
		return false;
	}
	return true;
}

bool PathManager::ReplacePak(QString &path, QString &oldName, QString &newName)
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
					if(!ReplacePakNameInJava(curFi->absoluteFilePath(), oldName, newName) || !ReplacePakNameInXml(curFi->absoluteFilePath(), oldName, newName)){
						return false;
					}
				}
				curFi++;
			}//end of while  
		}
	}

	if (!RenamePak(path, oldName, newName)){
		return false;
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

bool PathManager::ReplaceStr(QString &fileName, QString &beforeStr, QString &afterStr)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}
	QTextStream in(&file);
	in.setCodec("UTF-8");
	QString content;
	content = in.readAll();
	content.replace(beforeStr, afterStr);
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

bool PathManager::ReplacePakNameInXml(QString &fileName, QString &oldName, QString &newName)
{
	if (fileName.toLower().endsWith("xml")){
		QString oldNameTmp = QString("\"") + oldName + QString("\"");
		QString newNameTmp = QString("\"") + newName + QString("\"");
		return ReplaceStr(fileName, oldNameTmp, newNameTmp);
	}
	return true;
}

bool PathManager::ReplacePakNameInJava(QString &fileName, QString &oldName, QString &newName)
{
	if (fileName.toLower().endsWith("java")){
		return ReplaceStr(fileName, oldName, newName);
	}
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
					SetJdkPath(*ite);
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