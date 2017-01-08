﻿// Bootstrapper_v2.cpp : Defines the entry point for the application.
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

	CTrace::SetLevel(4);

	ATLENSURE_SUCCEEDED(::CoInitialize(NULL));

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
	auto GetVarFromPrivateProfileString = [IniFilePath](LPCTSTR key)
	{
		CString value;
		::GetPrivateProfileString(_T("MetrControlDB"), key, NULL, value.GetBuffer(DEFAULT_STR_LENGTH), DEFAULT_STR_LENGTH, IniFilePath);
		value.ReleaseBuffer();
		return value;
	};

	CString Server = GetVarFromPrivateProfileString(_T("Server"));
	CString Database = GetVarFromPrivateProfileString(_T("Database"));
	CString Login = GetVarFromPrivateProfileString(_T("Login"));
	CString PasswordHash = GetVarFromPrivateProfileString(_T("PasswordHash"));
	CString NTLM = GetVarFromPrivateProfileString(_T("NTLM"));
	#pragma endregion

	#pragma region Запись файла конфигурации АИС Метрконтроль
	{ // для локализации и освобождения COM объектов до CoUninitialize
		CComHeapPtr<TCHAR> pszLocalAppDataPath;
		ATLENSURE_SUCCEEDED(::SHGetKnownFolderPath(
			FOLDERID_LocalAppData,
			CSIDL_PERSONAL | CSIDL_FLAG_CREATE,
			NULL,
			(PTSTR*)&pszLocalAppDataPath
		));
		CString ConfigFilePath(pszLocalAppDataPath);
		::PathAppend(ConfigFilePath.GetBuffer(MAX_PATH), _T("IFirst\\MetrControl\\CnnSettings.xml"));
		ConfigFilePath.ReleaseBuffer();
		ATLTRACE2(
			atlTraceGeneral, 4,
			_T("Путь файла конфигурации: \"%s\".\n"),
			ConfigFilePath.GetString()
		);

		MSXML2::IXMLDOMDocument2Ptr ConfigFileDoc(__uuidof(MSXML2::DOMDocument60));
		MSXML2::IXMLDOMNodePtr ConfigFileElement(ConfigFileDoc);
		auto xmlDeclaration = ConfigFileElement->appendChild((MSXML2::IXMLDOMNodePtr)(ConfigFileDoc->createProcessingInstruction(_T("xml"), _T("version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\""))));
		auto EntriesNode = ConfigFileElement->appendChild((MSXML2::IXMLDOMNodePtr)(ConfigFileDoc->createElement(_T("entries"))));

		auto AddConfigEntry = [ConfigFileDoc, EntriesNode] (LPCTSTR key, LPCTSTR value)
		{
			auto KeyNode = EntriesNode->appendChild((MSXML2::IXMLDOMNodePtr)(ConfigFileDoc->createElement(_T("entry"))));
			(KeyNode->attributes->setNamedItem((MSXML2::IXMLDOMNodePtr)(ConfigFileDoc->createAttribute(_T("key")))))->nodeTypedValue = key;
			KeyNode->nodeTypedValue = value;
		};

		AddConfigEntry(_T("Key"), _T("35AA16F410699668C19C86A111D7A1287FD6D0FB33490C9B"));
		AddConfigEntry(_T("IV"), _T("3CE48E956DFB399A"));
		AddConfigEntry(_T("iFirst.SP.Ryabkov.CrossSessionData.UniversalSettingsSaver key: Assembly version: "), _T("1.48.0.1"));
		AddConfigEntry(_T("SQLConnectLib.SQLConnecter key: server"), Server.GetString());
		AddConfigEntry(_T("SQLConnectLib.SQLConnecter key: db"), Database.GetString());
		AddConfigEntry(_T("SQLConnectLib.SQLConnecter key: user"), Login.GetString());
		AddConfigEntry(_T("SQLConnectLib.SQLConnecter key: passwd"), PasswordHash.GetString());
		AddConfigEntry(_T("SQLConnectLib.SQLConnecter key: timeout"), _T("-1"));
		AddConfigEntry(_T("SQLConnectLib.SQLConnecter key: ntauth"), (NTLM == _T("yes")) ? _T("true") : _T("false"));

		ATLTRACE2(
			atlTraceGeneral, 4,
			_T("Подготовлено содержимое файла конфигурации:\n\n%s\n\n"),
			(LPCTSTR)(ConfigFileElement->xml)
		);
		ConfigFileDoc->save(ConfigFilePath.AllocSysString());
	};
	#pragma endregion

	::CoUninitialize();

	//	return ERROR_UNIDENTIFIED_ERROR;
    return ERROR_SUCCESS;
}
