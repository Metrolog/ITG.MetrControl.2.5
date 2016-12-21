// Bootstrapper_v2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Bootstrapper_v2.h"

int
APIENTRY
wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
	std::wstring appTitle;

	try
	{
		#pragma region читаем из ресурсов заголовок приложения
		std::unique_ptr<wchar_t[]> buffer;
		DWORD bufferLength = 256;
		buffer = std::make_unique<wchar_t[]>(bufferLength);
		::LoadString(hInstance, IDS_APP_TITLE, buffer.get(), bufferLength);
		if (ERROR_SUCCESS != ::GetLastError())
		{
			throw std::runtime_error(""); // TODO
		}
		appTitle = buffer.get();
		#pragma endregion

		#pragma region анализируем аргументы командной строки
		for (int i = 0; i<__argc; i++) {
			MessageBoxW(NULL, __wargv[i], appTitle.c_str(), MB_OK);
		}

		if ((2>__argc) || (3<__argc)) {
			throw std::invalid_argument(""); // TODO
		}
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
