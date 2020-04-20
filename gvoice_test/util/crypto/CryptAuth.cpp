#include "../crypto/libtomcrypt/mycrypt.h"
#include "CryptMath.h"
#include "CryptAuth.h"

using namespace std;

#if _PLATFORM_64_
#pragma  comment(lib,"../crypto/libtomcrypt/win/libtomcrypt64.lib")
#else
#pragma  comment(lib,"../crypto/libtomcrypt/win/libtomcrypt.lib")
#endif

void CCryptAuth::GetMD5Str(char* md5str, const char* str)
{
	return ::GetMD5Str(md5str, str);
}

int64 CCryptAuth::GetHash(const unsigned char* buffer, size_t len)
{
	return ::GetHash(buffer, len);
}

int64 CCryptAuth::GetHash(const char* str)
{
	return CCryptAuth::GetHash((BYTE*)str, strlen(str));
}

int64 CCryptAuth::GetHash(int64 passhash, int64 challenge)
{
//	NetDebug4("GetHash ", passhash, ",",challenge);
	return ::GetHash(passhash, challenge);
}

int64 CCryptAuth::RequestPassWord(const char* account, int64 challenge, bool force)
{
	CBiboScopedLock<CBiboLock> sl(&m_lock);
	AccountMap::iterator ai = m_accmap.find(account);
	if(ai!=m_accmap.end() && !force)
	{
		if(ai->second.pass!=-1)
		{
			int64 hash = CCryptAuth::GetHash(ai->second.pass, challenge);
			return hash;
		}
		else
		{
			m_accmap.erase(ai);//invalid entry, remove it to prevent memory leak
			return -1;
		}
	}
	else
	{
		//send DB request
		if(m_passdb)
		{
			m_passdb->LoadPass(account);
		}
		//
		return 0;
	}
}

int64 CCryptAuth::GetPassWordHash(const char* account, int64 challenge)
{
	CBiboScopedLock<CBiboLock> sl(&m_lock);
	AccountMap::iterator ai = m_accmap.find(account);
	if(ai!=m_accmap.end())
	{
		if(ai->second.pass!=-1)
		{
			int64 hash = CCryptAuth::GetHash(ai->second.pass, challenge);
			return hash;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return 0;
	}
}

int64 CCryptAuth::GenerateRandomNumber()
{
	CBiboScopedLock<CBiboLock> sl(&m_lock);
	int64 code = SecureRandom();
	return code;
}

void CCryptAuth::GeneratePuzzle(client_puzzle_t* p, int difficulty)
{
	CBiboScopedLock<CBiboLock> sl(&m_lock);
	hash_state md;
	sha256_init(&md);

	SecureRandom((BYTE*)&p->answer, sizeof(p->answer));
	sha256_process(&md, (BYTE*)&p->answer, sizeof(p->answer));
	sha256_done(&md, (BYTE*)p->hash_val);

	p->startp = p->answer>>difficulty<<difficulty;
	p->endp = p->startp+(1LL<<difficulty);
}

int CCryptAuth::SolvePuzzle(client_puzzle_t* p, int64* result)//return 1:ok, 0:continue, -1:fail
{
	int64 hash_val[4];
	for(int i=0;i<0x10000;++i,++p->startp)
	{
		if(p->startp > p->endp)
			return -1;
		hash_state md;
		sha256_init(&md);
		sha256_process(&md, (BYTE*)&p->startp, sizeof(p->startp));
		sha256_done(&md, (BYTE*)hash_val);

		if(p->hash_val[0]==hash_val[0]
		 &&p->hash_val[1]==hash_val[1]
		 &&p->hash_val[2]==hash_val[2]
		 &&p->hash_val[3]==hash_val[3]
		)
		{
			*result = p->startp;
			return 1;
		}
	}
	return 0;
}

void CCryptAuth::Encrypt(const BYTE *plainText, BYTE *cipherText, size_t len, int64 key)
{
	int64* c=(int64*)cipherText;
	int64* p=(int64*)plainText;
	len>>=3;
	for(size_t i=0;i<len;++i)
	{
//		int64 pp = p[i];
		c[i] = p[i]^key;
//		LogDebug("encrypt cipher = %I64x, text = %I64x, key = %I64x\n", c[i], pp, m_key);
	}
}

void CCryptAuth::Decrypt(const BYTE *cipherText, BYTE *plainText, size_t len, int64 key)
{
	int64* c=(int64*)cipherText;
	int64* p=(int64*)plainText;
	len>>=3;
	for(size_t i=0;i<len;++i)
	{
//		int64 cc=c[i];
		p[i] = c[i]^key;
//		LogDebug("decrypt cipher = %I64x, text = %I64x, key = %I64x\n", cc, p[i], m_key);
	}
}

void CCryptAuth::AddDirectPassword( int64 pass, tick_t time_out )
{
	NetInfo("add passwd entry: ", pass, ", time_out: ", time_out);
	CBiboScopedLock<CBiboLock> sl(&m_lock);
	pass_entry_t entry;
	entry.pass = pass;
	entry.start_time = get_tick_count();
	entry.time_out = time_out;

	m_passes[pass] = entry;
}

void CCryptAuth::RemoveDirectPassword( int64 pass )
{
	CBiboScopedLock<CBiboLock> sl(&m_lock);
	m_passes.erase(pass);
}

int64 CCryptAuth::FindDirectPassword( int64 pass )
{
	CBiboScopedLock<CBiboLock> sl(&m_lock);

	CheckTimeOut();

	DPassMap::iterator pi = m_passes.find(pass);
	if(pi == m_passes.end())
		return 0;
	else
		return pi->second.pass;
}

void CCryptAuth::CheckTimeOut()
{
	tick_t tick = get_tick_count();
	if(tick - m_last_check > 1000)
	{
		m_last_check = tick;

		for(AccountMap::iterator ai = m_accmap.begin(); ai != m_accmap.end();)
		{
			tick_t timeout = ai->second.time_out;
			if(timeout!=0 && tick - ai->second.start_time > timeout)
			{
				AccountMap::iterator cur = ai++;
				m_accmap.erase(cur);
			}
			else
			{
				++ai;
			}
		}

		for(DPassMap::iterator pi = m_passes.begin(); pi != m_passes.end();)
		{
			tick_t timeout = pi->second.time_out;
			if(timeout!=0 && tick - pi->second.start_time > timeout)
			{
				DPassMap::iterator cur = pi++;
				m_passes.erase(cur);
			}
			else
			{
				++pi;
			}
		}
	}
}

void CCryptAuth::AddAccount( const char* str, int64 passwd_hash, tick_t time_out )
{
	CBiboScopedLock<CBiboLock> sl(&m_lock);

	pass_entry_t entry;
	entry.pass = passwd_hash;
	entry.start_time = get_tick_count();
	entry.time_out = time_out;

	m_accmap[str] = entry;
}

void CCryptAuth::UpdateAuthentication()
{
	if(m_passdb)
	{
		m_passdb->UpdatePasswdDB();
	}
}
