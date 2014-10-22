#ifndef PATH_MANAGER_H
#define PATH_MANAGER_H
#include <QString>
#include <QStandardPaths>
#include <QDir>

class  PathManager
{
public:
	static QString GetStartPath();
	static QString GetConfigPath();
	static QString GetDataBasePath();
	static QString GetToolPath();
	static QString GetApkToolPath();
	static QString GetSrcPack();
	static void CreatDir(QString &dir);
	static void SetOutPath(QString &path);
	static QString GetOutPath();
	static void SetDecPackPath(QString &path);
	static QString GetDecPackPath();
	static void SetKeyPath(QString &path);
	static QString GetKeyPath();
	static void SetJdkPath(QString &path);
	static QString GetJdkPath();
	static void SetSrcPath(QString &path);
	static QString GetSrcPath();
	static QString GetSdkPath();
	static QString GetSdkToolsPath();
	static QString GetPrePack();
	static QString GetZipalign();
	static QString GetAntPath();
	static QString GetAnt();
	static QString GetAndroid();
	static QString GetBin(QString &projectPath);
	static QString GetReleaseApk(QString &path);
	static QString GetReleaseUnsignApk(QString &path);
	static QString GetJarSigner();
	static bool AppendContentToProperties(QString &content,QString &path);
	static QString GetTarget(QString &path);
	static void SetSdkPath(QString &path);
	static void SetPasswd(QString &password);
	static QString GetPasswd();
	static QString GetTmpPath();
	static bool RemoveDir(const QString &dirName);
	static void WriteSetting();
	static void ReadSetting();
	static void WriteLastPath(QString &key,QString &val);
	static QString ReadLastPath(QString &key);
	static bool CopyDir(const QString &source, const QString &destination, bool isCover);
	static bool CopyFile(const QString &SrcFile, const QString &DestFile, bool isCover);
	static bool SearchDirContianSuffix(const QString &dirFrom, QStringList &result, QString &suffix);
	static bool ReplaceStr(QString &fileName, QString &beforeStr, QString &afterStr);
	static bool ReplacePakNameInXml(QString &fileName,QString &oldName,QString &newName);
	static bool ReplacePakNameInJava(QString &fileName, QString &oldName, QString &newName);
	static bool RenamePak(QString &path, QString &oldName, QString &newName);
	static bool ReplacePak(QString &path, QString &oldName, QString &newName);
	static void SetKeyAliases(QString &aliases);
	static QString GetKeyAliases();
	static void SetThreadNum(int num);
	static int GetThreadNum();
	static void SetVersion(QString &ver);
	static QString GetVersion();
	static void SetPackWay(int way);
	static int GetPackWay();
	static void SetSigalg(QString &sig);
	static QString GetSigalg();
	static void SetDigestalg(QString &dig);
	static QString GetDigestalg();
	static bool CheckSysEnvironment();
	static bool CheckParameter();
protected:
private:
	static QString OUTPATH;
	static QString DECPACKPATH;
	static QString KEYPATH;
	static QString JDKPATH;
	static QString SRCPATH;
	static QString SDKPATH;
	static QString PASSWD;
	static QString KEYALIASES;
	static QString VERSION;
	static QString SIGALG;
	static QString DIGESTALG;
	static int THREADNUM;
	static int PACKWAY;
	
};
#endif