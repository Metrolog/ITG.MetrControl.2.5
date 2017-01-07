// Bootstrapper_v2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Bootstrapper_v2.h"

#define DEFAULT_STR_LENGTH 0x0400

int APIENTRY wWinMain
(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPTSTR lpCmdLine,
  _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	CString appTitle = _T("Metrcontrol loader");
	appTitle.LoadString(IDS_APP_TITLE);

	#pragma region анализируем аргументы командной строки
	if ((2 > __argc) || (3 < __argc))
	{
		return ERROR_BAD_ARGUMENTS;
	}

	CString IniFilePath(__targv[1]);
	bootstrapper::csmToolId ToolId = bootstrapper::csmToolId::csmmain;
	if (3 == __argc)
	{
		if (IniFilePath == _T("csmmain")) ToolId = bootstrapper::csmToolId::csmmain;
		else if (IniFilePath == _T("csmadmin")) ToolId = bootstrapper::csmToolId::csmadmin;
		else if (IniFilePath == _T("markinv")) ToolId = bootstrapper::csmToolId::markinv;
		else
		{
			return ERROR_BAD_ARGUMENTS;
		}
	}
	#pragma endregion

	#pragma region Читаем ini файл дескриптора базы
	static LPCWSTR IniSection = L"MetrControlDB";

	#define GetVarFromPrivateProfileString(Var) \
		CString Var; \
		::GetPrivateProfileString(IniSection, _T(# Var), NULL, Var.GetBuffer(DEFAULT_STR_LENGTH), DEFAULT_STR_LENGTH, IniFilePath); \
		Var.ReleaseBuffer();

	GetVarFromPrivateProfileString(Server);
	GetVarFromPrivateProfileString(Database);
	GetVarFromPrivateProfileString(Login);
	GetVarFromPrivateProfileString(PasswordHash);
	GetVarFromPrivateProfileString(NTLM);
	#pragma endregion

	#pragma region Запись файла конфигурации АИС Метрконтроль
	CStringW ConfigFileContent;
	ConfigFileContent.FormatMessageW(
		IDS_CONFIG_FILE_TEMPLATE
		, Server.GetString()
		, Database.GetString()
		, Login.GetString()
		, PasswordHash.GetString()
		, (NTLM == _T("yes")) ? _T("true") : _T("false")
	);
	MessageBox(NULL, ConfigFileContent, appTitle, MB_OK);

	// https://msdn.microsoft.com/en-us/library/bb762188.aspx
	#pragma endregion

	//	return ERROR_UNIDENTIFIED_ERROR;
    return ERROR_SUCCESS;
}
