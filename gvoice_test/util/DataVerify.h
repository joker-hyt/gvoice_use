#pragma once
#include "crypto/libtomcrypt/mycrypt.h"

class CDataVerify
{
public:
	static void GetMD5(char* dest, const char* source, int size)
	{
		hash_state md;
		md5_init(&md);

		md5_process(&md, (const unsigned char*)source, size);

		md5_done(&md, (unsigned char*)dest);
	}
	static void ConvertMD5toReadableString(char *szStr, char *szOut)
	{
		int i = 0;
		char szTmp[4];
		szOut[0] = 0x0;
		for (i = 0; i < 16; i++)
		{
			sprintf(szTmp, "%02x", (unsigned char)szStr[i]);
			strcat(szOut, szTmp);
		}
	}
};
