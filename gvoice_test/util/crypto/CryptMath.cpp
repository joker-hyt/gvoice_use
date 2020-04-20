#include "../crypto/libtomcrypt/mycrypt.h"
#include "CryptMath.h"

using namespace std;


void GetMD5Str(char* md5str, const char* str)
{
	unsigned char md5[16];
	hash_state md;
	md5_init(&md);

	md5_process(&md, (BYTE*)str, (unsigned long)strlen(str));

	md5_done(&md, (BYTE*)md5);

	md5str[0] = 0;
	for(int i=0;i<16;++i)
	{
		char digit[4];
		sprintf(digit, "%02x", md5[i]);
		strcat(md5str, digit);
	}
}

void GetHash(BYTE* phash, const void* buffer, size_t len)
{
	if(buffer==0)
		return;

	hash_state md;
	sha256_init(&md);

	sha256_process(&md, (BYTE*)buffer, (unsigned long)len);

	sha256_done(&md, phash);
}

int64 GetHash(const void* buffer, size_t len)
{
	if(buffer==0)
		return 0;

	int64 hash_val[4];

	hash_state md;
	sha256_init(&md);

	sha256_process(&md, (BYTE*)buffer, (unsigned long)len);

	sha256_done(&md, (BYTE*)hash_val);

	return hash_val[0]^hash_val[1]^hash_val[2]^hash_val[3];
}

int64 GetHash(int64 v1, int64 v2)
{
	int64 hash_val[4];

	hash_state md;
	sha256_init(&md);

	sha256_process(&md, (BYTE*)&v1, sizeof(v1));
	sha256_process(&md, (BYTE*)&v2, sizeof(v2));

	sha256_done(&md, (BYTE*)hash_val);

	return hash_val[0]^hash_val[1]^hash_val[2]^hash_val[3];
}


int InitSecureRandom(prng_state* prng, unsigned long long entropy=0)
{
	int error;
	/* register yarrow */
	if (register_prng(&yarrow_desc) == -1) {
		LogCritical("Error registering Yarrow");
		return -1;
	}
	/* setup the PRNG */
	if ((error = rng_make_prng(128, find_prng("yarrow"), prng, NULL)) != CRYPT_OK) {
		LogCritical("Error setting up PRNG, ", error_to_string(error));
		return -1;
	}
	if ((error = yarrow_add_entropy((BYTE*)&entropy, sizeof(entropy), prng)) != CRYPT_OK) {
		LogCritical("Add_entropy error: ", error_to_string(error));
		return -1;
	}
	/* ready and read */
	if ((error = yarrow_ready(prng)) != CRYPT_OK) {
		LogCritical("Ready error: ", error_to_string(error));
		return -1;
	}
	return 0;
}

int SecureRandom(BYTE* buff, int len, unsigned long long entropy)
{
	static prng_state prng;
	static bool init = false;
	if(!init)
	{
		if(InitSecureRandom(&prng, entropy)!=0)
			return -1;
		init = true;
	}
	return yarrow_read(buff, len, &prng);
}

int GenerateECCKey(ecc_key* key, unsigned long long entropy=0)
{
	prng_state prng;
	if(InitSecureRandom(&prng, entropy)!=0)
		return -1;
	int error;
	if ((error = ecc_make_key(&prng, find_prng("yarrow"), 32, key)) != CRYPT_OK) {
		LogCritical("Error making key: ", error_to_string(error));
		return -1;
	}
	return 0;
}

class ASymmetricKey
{
	enum{
		KeyLen = 20,
	};
public:
	ecc_key m_private_key;
	ecc_key m_public_key;

	BYTE m_buf[4096];
	unsigned long m_buflen;

	BYTE m_key[32];
	unsigned long m_keylen;

	static int sm_count;
	static prng_state sm_prng;

	bool m_is_valid;
	bool m_established;
	ASymmetricKey()
	{
		if(sm_count++==0)
		{
			InitSecureRandom(&sm_prng);
		}
		m_is_valid = false;
		m_established = false;

		int error;
		if ((error = ecc_make_key(&sm_prng, find_prng("yarrow"), KeyLen, &m_private_key)) != CRYPT_OK){
			LogCritical("Error making key: ", error_to_string(error));
			return;
		}

		m_buflen=sizeof(m_buf);
		if((error = ecc_export(m_buf, &m_buflen, PK_PUBLIC, &m_private_key)) != CRYPT_OK){
			LogCritical("Error exporting key: ", error_to_string(error));
			return;
		}

		m_is_valid = true;
	}
	~ASymmetricKey()
	{
		if(m_is_valid)
			ecc_free(&m_private_key);
		if(m_established)
			ecc_free(&m_public_key);
	}
	int Load(BYTE* inbuf, unsigned long buflen)
	{
		int error;
		if((error = ecc_import(inbuf, buflen, &m_public_key)) != CRYPT_OK){
			LogCritical("Error importing key: ", error_to_string(error));
			return -1;
		}

		BYTE buf[4096];
		unsigned long x = sizeof(buf);
		if ((error = ecc_shared_secret(&m_private_key, &m_public_key, buf, &x)) != CRYPT_OK) {
			LogCritical("Error importing key: ", error_to_string(error));
			return -1;
		}
		m_keylen = 32;
		if ((error = hash_memory(find_hash("sha256"), buf, x, m_key, &m_keylen)) != CRYPT_OK) {
			LogCritical("Error hashing memory: ", error_to_string(error));
			return -1;
		}
		m_established = true;
		return 0;
	}
};
int ASymmetricKey::sm_count = 0;
prng_state ASymmetricKey::sm_prng;

struct client_puzzle_t
{
	unsigned long long answer;

	BYTE hash_val[32];

	unsigned long long startp;
	unsigned long long endp;
};

void GeneratePuzzle(client_puzzle_t* p, int difficulty)
{
	hash_state md;
	sha256_init(&md);

	SecureRandom((BYTE*)&p->answer, sizeof(p->answer));
	sha256_process(&md, (BYTE*)&p->answer, sizeof(p->answer));
	sha256_done(&md, p->hash_val);

	p->startp = p->answer>>difficulty<<difficulty;
	p->endp = p->startp+(1LL<<difficulty);
}

int SolvePuzzle(client_puzzle_t* p, long long* res)
{
	BYTE hash_val[32];
	for(unsigned long long i=p->startp; i<=p->endp; ++i)
	{
		hash_state md;
		sha256_init(&md);
		sha256_process(&md, (BYTE*)&i, sizeof(p->answer));
		sha256_done(&md, hash_val);
		for(int j=0;j<32;++j)
		{
			if(hash_val[j]!=p->hash_val[j])
				goto fail;
		}
//succeeded
		*res = i;
		return 1;
fail:
		continue;
	}
	return 0;
}
