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

// 使用AES对数据加密时需要提供的最小输出buffer的长度
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
	//aes加密，outsize长度需要是input的16的倍数
	int needSize = GetAesEncryptNeedOutBufSize(inputSize);
	if(needSize > outputSize)
	{
		strongAssert(0);
		return false;
	}
	
	//需要补齐的数据pkcs5,少多少位，就补多少个n
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
	//aes解密，inputSize需要是16的整数倍，且outputSize不小于inputSize
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

	//去掉pkcs5编码相关的数据

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
