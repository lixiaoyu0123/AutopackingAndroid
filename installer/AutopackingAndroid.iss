; We use CMake's configure_file command to replace ${VAR_NAME} variables
; with actual values. Note the dollar sign; {VAR_NAME} variables are from
; Inno, the ones with the dollar we define with CMake.

[Setup]
AppId={3736C7B6-53BD-491F-AD3B-6B066F62787D}
AppName=android批量打包工具
AppVerName=AutopackingAndroid ${TOOLS_FULL_VERSION}
DefaultDirName={pf}\AutopackingAndroid
DefaultGroupName=AutopackingAndroid
UninstallDisplayIcon={app}\AutopackingAndroid.exe
AppPublisher=BingJian
AppPublisherURL=
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
Name: "{group}\AutopackingAndroid"; Filename: "{app}\AutopackingAndroid.exe"
Name: "{group}\用户手册"; Filename: "{app}\UserManual.pdf"
Name: "{group}\卸载AutopackingAndroid"; Filename: "{uninstallexe}"
Name: "{commondesktop}\AutopackingAndroid"; Filename: "{app}\AutopackingAndroid.exe"; Tasks: desktopicon
Name: "{userdesktop}\AutopackingAndroid"; Filename:"{app}\AutopackingAndroid.exe"

[InstallDelete]
Type: files; Name: "{group}\AutopackingAndroid 3.4.1.lnk";Type: files; Name: "{commonappdata}\Microsoft\Windows\Start Menu\Programs\AutopackingAndroid\AutopackingAndroid 3.4.1.lnk"

