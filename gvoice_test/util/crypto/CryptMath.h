#ifndef __CRYPTMATH_H__8bu998klo123__
#define __CRYPTMATH_H__8bu998klo123__

//#include "libtomcrypt/mycrypt.h"
#include "../../share/netengine/BiboFrame/BiboInterfaces.h"

int SecureRandom(BYTE* buff, int len, unsigned long long entropy=0);

inline int64 SecureRandom()
{
	int64 num=0;
	SecureRandom((BYTE*)&num, sizeof(num));
	return num;
}

void GetMD5Str(char* md5str, const char* str);

int64 GetHash(const void* buffer, size_t len);

int64 GetHash(int64 v1, int64 v2);

union hash_value_t
{
	char c[32];
	int64 i[4];
};

//phash����ָ�����32�ֽڵ��ڴ�
void GetHash(BYTE* phash, const void* buffer, size_t len);

struct CHashEncrypt
{
	//���뱣֤д���buffer����8���ֽ�
	void Encrypt(const int64 *input, hash_value_t *output)
	{
		GetHash((BYTE*)output, input, sizeof(int64));
	}
};

template<class T_ECB>
class CTR_TextEncryptor
{
	int64 m_counter;
	int64 m_ctr;
	T_ECB m_ecb;

	//�����ǳߴ����8�ֽڵ���Ч�ڴ�
	void ProcessBlock(const void *plainBlock, void *cipherBlock)
	{
		++m_counter;

		int64 input = m_ctr + m_counter;
		hash_value_t key;
		m_ecb.Encrypt(&input, &key);

		const hash_value_t* p = (const hash_value_t*)plainBlock;
		hash_value_t* c = (hash_value_t*)cipherBlock;
		
		c->i[0] = p->i[0]^key.i[0];
		c->i[1] = p->i[1]^key.i[1];
		c->i[2] = p->i[2]^key.i[2];
		c->i[3] = p->i[3]^key.i[3];
	}
public:
	CTR_TextEncryptor()
	{
		SetKey(0);
	}
	void SetKey(int64 key)
	{
		m_ctr = key;
		m_counter = 0;
	}
	int64 GetCounter()
	{
		return m_counter;
	}
	void Process(const char *input, char *output, const size_t len)
	{
		hash_value_t* c=(hash_value_t*)output;
		const hash_value_t* p=(const hash_value_t*)input;

		size_t block_count = len>>5;//������block�ĸ���
		//����������block
		for(size_t i=0; i<block_count; ++i)
		{
			ProcessBlock(p, c);
			++p;
			++c;
		}
		//ʣ����ֽڴ���8�ֽڴ���
		size_t left = len - (block_count<<5);
		if(left)
		{
			hash_value_t pt;
			hash_value_t ct;
			memset(&pt, 0, sizeof(pt));
			memset(&ct, 0, sizeof(ct));

			memcpy(&pt, p, left);
			ProcessBlock(&pt, &ct);
			memcpy(c, &ct, left);
		}
	}
};

#endif
