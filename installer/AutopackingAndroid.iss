; We use CMake's configure_file command to replace ${VAR_NAME} variables
; with actual values. Note the dollar sign; {VAR_NAME} variables are from
; Inno, the ones with the dollar we define with CMake.

[Setup]
AppId={{3736C7B6-53BD-491F-AD3B-6C066F627880}
AppName=android批量打包工具
AppVerName=Autopacking ${TOOLS_FULL_VERSION}
DefaultDirName={pf}\Autopacking
DefaultGroupName=Autopacking
UninstallDisplayIcon={app}\Autopacking.exe
AppPublisher=BingJian
AppPublisherURL=www.bingjian.com
WizardImageFile=compiler:wizmodernimage-IS.bmp
WizardSmallImageFile=compiler:wizmodernsmallimage-IS.bmp
Compression=lzma2
SolidCompression=yes  
OutputDir=..\installer
LicenseFile=${LICENSE_LOCATION}
; WinXP is the lowest supported version
MinVersion=0,5.1
PrivilegesRequired=admin
OutputBaseFilename=AutopackingAndroid-Windows${ISS_SETUP_FILENAME_PLATFORM}-${TOOLS_FULL_VERSION}
ChangesAssociations=yes

; "ArchitecturesAllowed=x64" specifies that Setup cannot run on
; anything but x64.
; The ${ISS_ARCH} var is substituted with "x64" or an empty string
ArchitecturesAllowed="${ISS_ARCH}"
; "ArchitecturesInstallIn64BitMode=x64" requests that the install be
; done in "64-bit mode" on x64, meaning it should use the native
; 64-bit Program Files directory and the 64-bit view of the registry.
; The ${ISS_ARCH} var is substituted with "x64" or an empty string
ArchitecturesInstallIn64BitMode="${ISS_ARCH}"

[Files]
Source: "AutopackingAndroid\*"; DestDir: "{app}"; Flags: createallsubdirs recursesubdirs ignoreversion
Source: vendor\vcredist.exe; DestDir: {tmp}

[Languages]
Name: zh; MessagesFile: compiler:Languages\ChineseSimplified.isl

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Icons]
Name: "{group}\AutopackingAndroid"; Filename: "{app}\Autopacking.exe"
Name: "{group}\用户手册"; Filename: "{app}\Documents\UserManual.pdf"
Name: "{group}\卸载AutopackingAndroid"; Filename: "{uninstallexe}"
Name: "{commondesktop}\AutopackingAndroid"; Filename: "{app}\Autopacking.exe"; Tasks: desktopicon
Name: "{userdesktop}\AutopackingAndroid"; Filename:"{app}\Autopacking.exe"

[Run]
Filename: {tmp}\vcredist.exe; Parameters: "/passive /Q:a /c:""msiexec /qb /i vcredist.msi"" "; StatusMsg: Installing 2010 RunTime...

[InstallDelete]
Type: files; Name: "{group}\Autopacking ${TOOLS_FULL_VERSION}.lnk";Type: files; Name: "{commonappdata}\Microsoft\Windows\Start Menu\Programs\AutopackingAndroid\AutopackingAndroid 3.4.1.lnk"

[Code]
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usUninstall then
    if MsgBox('您是否要保留Autopacking配置信息？', mbConfirmation, MB_YESNO) = IDNO then
      DelTree(ExpandConstant('{%UserProfile}\AppData\Local\bingjian'), True, True, False);
end;