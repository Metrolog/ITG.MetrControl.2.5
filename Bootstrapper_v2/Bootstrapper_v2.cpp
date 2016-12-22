// Bootstrapper_v2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Bootstrapper_v2.h"

#define DEFAULT_STR_LENGTH 1024

std::wstring GetValueFromCsmDbIni
(
	LPCWSTR FilePath,
	LPCWSTR ValueId
);

int APIENTRY wWinMain
(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	LPCWSTR appTitle = L"Metrcontrol loader";

	try
	{
		#pragma region читаем из ресурсов заголовок приложения
		WCHAR appTitleBuf[DEFAULT_STR_LENGTH];
		::LoadString(hInstance, IDS_APP_TITLE, appTitleBuf, DEFAULT_STR_LENGTH);
		if (ERROR_SUCCESS != ::GetLastError())
		{
			throw std::runtime_error(""); // TODO
		}
		appTitle = appTitleBuf;
		#pragma endregion

		#pragma region анализируем аргументы командной строки
		if ((2 > __argc) || (3 < __argc))
		{
			throw std::invalid_argument(""); // TODO
		}

		LPCWSTR IniFilePath = __wargv[1];
		bootstrapper::csmToolId ToolId = bootstrapper::csmToolId::csmmain;
		if (3 == __argc)
		{
			if (0 == std::wcscmp(IniFilePath, L"csmmain")) ToolId = bootstrapper::csmToolId::csmmain;
			else if (0 == std::wcscmp(IniFilePath, L"csmadmin")) ToolId = bootstrapper::csmToolId::csmadmin;
			else if (0 == std::wcscmp(IniFilePath, L"markinv")) ToolId = bootstrapper::csmToolId::markinv;
			else
			{
				throw std::invalid_argument(""); // TODO
			}
		}
		#pragma endregion

		#pragma region Читаем ini файл дескриптора базы
		const std::wstring Server		= GetValueFromCsmDbIni(IniFilePath, L"Server");
		std::wstring Database		= GetValueFromCsmDbIni(IniFilePath, L"Database");
		std::wstring Login		= GetValueFromCsmDbIni(IniFilePath, L"Login");
		std::wstring PasswordHash	= GetValueFromCsmDbIni(IniFilePath, L"PasswordHash");
		std::wstring NTLM			= GetValueFromCsmDbIni(IniFilePath, L"NTLM");

		std::wstringstream msgstream;
		msgstream << L"Server: " << Server << std::endl
			<< L"Database: " << Database << std::endl
			<< L"Login: " << Login << std::endl
			<< L"PasswordHash: " << PasswordHash << std::endl
			<< L"NTLM: " << NTLM;
		std::wstring msg = msgstream.str();
		MessageBoxW(NULL, msg.c_str(), appTitle, MB_OK);
		#pragma endregion

	}
	catch (const std::invalid_argument& e)
	{
		return ERROR_BAD_ARGUMENTS;
	}
	catch (const std::exception& e)
	{
		return ERROR_UNIDENTIFIED_ERROR;
	}
	catch (...)
	{
		return ERROR_UNIDENTIFIED_ERROR;
	}
    return ERROR_SUCCESS;
}

std::wstring GetValueFromCsmDbIni
(
	LPCWSTR FilePath,
	LPCWSTR ValueId
)
{
	static LPCWSTR IniSection = L"MetrControlDB";

	WCHAR result[DEFAULT_STR_LENGTH];
	::GetPrivateProfileStringW(IniSection, ValueId, NULL, result, DEFAULT_STR_LENGTH, FilePath);
	if (ERROR_SUCCESS != ::GetLastError())
	{
		return L"";
		// throw std::runtime_error(""); // TODO
	}

	return result;
};
