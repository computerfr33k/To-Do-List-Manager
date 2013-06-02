; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{F50D5DE5-74DB-43F9-9D5D-0731FFEC4459}
AppName=To Do List Manager
AppVersion=0.5
;AppVerName=To Do List Manager 0.3
AppPublisher=Computerfr33k
AppPublisherURL=http://www.computerfr33k.com/
AppSupportURL=http://www.computerfr33k.com/
AppUpdatesURL=http://www.computerfr33k.com/
DefaultDirName={pf}\To Do List Manager
DefaultGroupName=To Do List Manager
LicenseFile=C:\Users\Eric\Documents\To-Do-List-Manager\LICENSE.txt
OutputBaseFilename=to-do-list-manager-setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "C:\Users\Eric\Documents\GitHub\To-Do-List-Manager-build-Desktop_Qt_5_0_1_MSVC2010_32bit-Release\release\To-Do-List-Manager.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Eric\Documents\GitHub\To-Do-List-Manager-build-Desktop_Qt_5_0_1_MSVC2010_32bit-Release\release\*"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Eric\Documents\GitHub\To-Do-List-Manager-build-Desktop_Qt_5_0_1_MSVC2010_32bit-Release\release\sqldrivers\*"; DestDir: "{app}\sqldrivers\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\Eric\Documents\GitHub\To-Do-List-Manager-build-Desktop_Qt_5_0_1_MSVC2010_32bit-Release\release\platforms\*"; DestDir: "{app}\platforms\"; Flags: ignoreversion recursesubdirs createallsubdirs
;Source: "C:\Qt\Qt5.0.1\vcredist\vcredist_sp1_x86.exe"; DestDir: {tmp}; Flags: deleteafterinstall
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\To Do List Manager"; Filename: "{app}\To-Do-List-Manager.exe"
Name: "{group}\{cm:UninstallProgram,To Do List Manager}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\To Do List Manager"; Filename: "{app}\To-Do-List-Manager.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\To Do List Manager"; Filename: "{app}\To-Do-List-Manager.exe"; Tasks: quicklaunchicon

[Run]
;Filename: {tmp}\vcredist_sp1_x86.exe; Parameters: "/passive /Q:a /c:""msiexec /qb /i vcredist.msi"" "; StatusMsg: Installing 2010 RunTime...; Check: VCRedistNeedsInstall
;Filename: "{tmp}\vcredist_sp1_x86.exe"; Check: VCRedistNeedsInstall
Filename: "{app}\To-Do-List-Manager.exe"; Description: "{cm:LaunchProgram,To Do List Manager}"; Flags: nowait postinstall skipifsilent

[Code]
/////////////////////////////////////////////////////////////////////
function GetUninstallString(): String;
var
  sUnInstPath: String;
  sUnInstallString: String;
begin
  sUnInstPath := ExpandConstant('Software\Microsoft\Windows\CurrentVersion\Uninstall\{#emit SetupSetting("AppId")}_is1');
  sUnInstallString := '';
  if not RegQueryStringValue(HKLM, sUnInstPath, 'UninstallString', sUnInstallString) then
    RegQueryStringValue(HKCU, sUnInstPath, 'UninstallString', sUnInstallString);
  Result := sUnInstallString;
end;


/////////////////////////////////////////////////////////////////////
function IsUpgrade(): Boolean;
begin
  Result := (GetUninstallString() <> '');
end;


/////////////////////////////////////////////////////////////////////
function UnInstallOldVersion(): Integer;
var
  sUnInstallString: String;
  iResultCode: Integer;
begin
// Return Values:
// 1 - uninstall string is empty
// 2 - error executing the UnInstallString
// 3 - successfully executed the UnInstallString

  // default return value
  Result := 0;

  // get the uninstall string of the old app
  sUnInstallString := GetUninstallString();
  if sUnInstallString <> '' then begin
    sUnInstallString := RemoveQuotes(sUnInstallString);
    if Exec(sUnInstallString, '/SILENT /NORESTART /SUPPRESSMSGBOXES','', SW_HIDE, ewWaitUntilTerminated, iResultCode) then
      Result := 3
    else
      Result := 2;
  end else
    Result := 1;
end;

/////////////////////////////////////////////////////////////////////
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep=ssInstall) then
  begin
    if (IsUpgrade()) then
    begin
      UnInstallOldVersion();
    end;
  end;
end;