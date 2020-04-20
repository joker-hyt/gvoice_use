#include "aes_encryption.h"
#include "libtomcrypt/mycrypt.h"
CAesEncryption::CAesEncryption()
:m_init(false)
{

}

CAesEncryption::~CAesEncryption()
{

}

void CAesEncryption::Init()
{
	m_init = true;
	register_cipher(&aes_desc);
}

void CAesEncryption::UnInit()
{
	if(!m_init)
		return;

	unregister_cipher(&aes_desc);
	m_init = false;

}

bool CAesEncryption::IsInit()
{
	return m_init;
}

// ʹ��AES�����ݼ���ʱ��Ҫ�ṩ����С���buffer�ĳ���
int CAesEncryption::GetAesEncryptNeedOutBufSize(int inputSize)
{
	return inputSize + (16 - inputSize % 16);
}

bool CAesEncryption::Encrypt(const char* key, const unsigned char *input, unsigned char *output)
{
	strongAssert(strlen((const char*)input) <= 16);


	symmetric_key sKey;
	int s = strlen(key);
	int res = aes_setup((unsigned const char*)key, s, 0, &sKey);
	if(res != CRYPT_OK)
	{
		return false;
	}
	res = aes_ecb_encrypt(input, output, &sKey);
	if(res != CRYPT_OK)
	{
		return false;
	}
	aes_done(&sKey);
	return true;
}

bool CAesEncryption::Encrypt(const char* key, const unsigned char* input, int inputSize, 
							 unsigned char* output, int outputSize, int& outLen)
{
	symmetric_key sKey;
	int s = strlen(key);
	int res = aes_setup((unsigned const char*)key, s, 0, &sKey);
	if(res != CRYPT_OK)
	{
		return false;
	}
	//aes���ܣ�outsize������Ҫ��input��16�ı���
	int needSize = GetAesEncryptNeedOutBufSize(inputSize);
	if(needSize > outputSize)
	{
		strongAssert(0);
		return false;
	}
	
	//��Ҫ���������pkcs5,�ٶ���λ���Ͳ����ٸ�n
	int needPk = needSize - inputSize;

	unsigned char* tmp = new unsigned char[needSize];
	memset(tmp, needPk, needSize);
	memcpy(tmp, input, inputSize);

	int idx = 0;
	bool result = true;
	int lastErr = 0;
	while(idx < needSize)
	{
		lastErr = aes_ecb_encrypt(tmp+idx, output+idx, &sKey);
		if(lastErr != CRYPT_OK)
		{
			result = false;
			break;
		}
		idx+=16;
	}
	aes_done(&sKey);
	outLen = needSize;
	
	delete[] tmp;
	return result;
}

bool CAesEncryption::Decrypt(const char* key, const unsigned char *input, unsigned char *output)
{

	symmetric_key sKey;
	int s = strlen(key);
	int res = aes_setup((unsigned const char*)key, s, 0, &sKey);

	if(res != CRYPT_OK)
	{
		return false;
	}

	res = aes_ecb_decrypt(input, output, &sKey);

	if(res != CRYPT_OK)
	{
		return false;
	}
	aes_done(&sKey);
	return true;
}

bool CAesEncryption::Decrypt(const char* key, const unsigned char* input, int inputSize, 
							 unsigned char* output, int outputSize, int& outLen)
{
	symmetric_key sKey;
	int s = strlen(key);
	int res = aes_setup((unsigned const char*)key, s, 0, &sKey);

	if(res != CRYPT_OK)
	{
		return false;
	}
	//aes���ܣ�inputSize��Ҫ��16������������outputSize��С��inputSize
	if((inputSize & 0x0F) != 0 || inputSize > outputSize)
	{
		strongAssert(0);
		return false;
	}

	int idx = 0;
	bool result = true;
	int lastErr = 0;
	while(idx < inputSize)
	{
		lastErr = aes_ecb_decrypt(input+idx, output+idx, &sKey);
		if(lastErr != CRYPT_OK)
		{
			result = false;
			break;
		}
		idx+=16;
	}
	aes_done(&sKey);

	//ȥ��pkcs5������ص�����

	char last = output[inputSize - 1];
	bool needRemove = true;
	for(int i = 1; i < last; i++)
	{
		if(output[inputSize -1 -i] != last)
		{
			needRemove = false;
			break;
		}
	}

	if(needRemove)
	{
		outLen = inputSize - last;
		output[inputSize - last] = 0;
	}
	else
	{
		outLen = inputSize;
	}
	return result;
}

int CAesEncryption::Base64Encode(const unsigned char *in,  unsigned int len, 
								 unsigned char *out, unsigned int *outlen)
{
	unsigned long data = 0;
	if(outlen)
		data = *outlen;	
	int res = base64_encode(in, len, out, &data);
	if(outlen)
	{
		*outlen = data;
	}
	return res;
}

int CAesEncryption::Base64Decode(const unsigned char *in,  unsigned int len, 
								 unsigned char *out, unsigned int *outlen)
{
	unsigned long data = 0;
	if(outlen)
		data = *outlen;

	int res = base64_decode(in, len, out, &data);
	if(outlen)
	{
		*outlen = data;
	}
	return res;
}
