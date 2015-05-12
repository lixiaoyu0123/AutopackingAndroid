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
	static QString GetDocumentsPath();
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
	static QString GetAntPath();
	static QString GetAnt();
	static QString GetAndroid();
	static QString GetBin(QString &projectPath);
	static QString GetReleaseApk(QString &path);
	static QString GetReleaseUnsignApk(QString &path);
	static bool AppendContentToProperties(QString &content,QString &path);
	static QString GetTarget(QString &path);
	static QStringList GetLibRef(QString &path);
	static void Deduplication(QStringList &list);
	static void CorrectLibRef(QString &path);
	static QString GetApkDir(QString &proFile);
	static void SetSdkPath(QString &path);
	static void SetPasswd(QString &password);
	static void SetAliasesPasswd(QString &password);
	static QString GetAliasesPasswd();
	static QString GetPasswd();
	static QString GetTmpPath();
	static QString GetDecTmpPath();
	static bool RemoveDir(const QString &dirName);
	static void WriteSetting();
	static void ReadSetting();
	static void WriteLastPath(QString &key,QString &val);
	static QString ReadLastPath(QString &key);
	static bool CopyDir(const QString &source, const QString &destination, bool isCover);
	static bool CopyFile(const QString &SrcFile, const QString &DestFile, bool isCover);
	static bool SearchDirContianSuffix(const QString &dirFrom, QStringList &result, QString &suffix);
	static bool RewriteApktoolYml(QString &fileName);
	static bool ReplaceStr(QString &fileName, QString &srcStr, QString &replaceStr);
	static int ReplaceStrStrict(QString &fileName, QString &srcStr, QString &replaceStr);
	static bool ReplaceByRegular(QString &regular, QString &text,QString &replaceStr);
	static bool ReplacePakNameInXml(QString &srcPath,QString &fileName,QString &oldName,QString &newName);
	static bool ReplaceAppPakNameInXml(QString &srcPath, QString &fileName, QString &oldName, QString &newName);
	static bool ReplaceAppPakNameInManifest(QString &path, QString &oldName, QString &newName);
	static bool ReplacePakNameInJava(QString &srcPath,QString &fileName, QString &oldName, QString &newName);
	static bool ReplaceAppPakNameInJava(QString &srcPath, QString &fileName, QString &oldName, QString &newName);
	static bool ReplacePakNameInSmali(QString &fileName, QString &oldName, QString &newName);
	static bool ReplaceAppPakNameInSmali(QString &fileName, QString &oldName, QString &newName);
	static void RemoveEmptyDirFromDeepest(QString &path);
	static int RenamePak(QString &path,QString &oldPakName, QString &newPakName);
	static int RenamePakInSrc(QString &path, QString &oldName, QString &newName);
	static int RenamePakInDec(QString &path, QString &oldName, QString &newName);
	static int ReplacePakInSrc(QString &path, QString &oldName, QString &newName);
	static int ReplacePakInDec(QString &path, QString &oldName, QString &newName);
	static int ReplaceAppPakInSrc(QString &path, QString &oldName, QString &newName);
	static int ReplaceAppPakInDec(QString &path, QString &oldName, QString &newName);
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
	static QString GetBuildXml();
	static bool IsFirstRun();
protected:
private:
	static QString OUTPATH;
	static QString DECPACKPATH;
	static QString KEYPATH;
	static QString JDKPATH;
	static QString SRCPATH;
	static QString SDKPATH;
	static QString PASSWD;
	static QString ALIASESPASSWD;
	static QString KEYALIASES;
	static QString VERSION;
	static QString SIGALG;
	static QString DIGESTALG;
	static int THREADNUM;
	static int PACKWAY;
	
};
#endif