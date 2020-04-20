#ifndef __TRIVAL_AUTH_H__kkksk8uhhklaj3rhbvi__
#define __TRIVAL_AUTH_H__kkksk8uhhklaj3rhbvi__

//#include "GameConnection.h"
#include <set>

class CTrivalAuth : public IAuthentication
{
public:
	typedef std::map<std::string, int64> PassMap;
	PassMap m_passmap;

	int m_count;
	CTrivalAuth()
	{
		m_count = 0;
	}
	virtual ~CTrivalAuth()
	{
	}

	void GetMD5Str(char* md5str, const char* str)
	{
		md5str[0] = 0;
	}
	virtual void Release()
	{
		delete this;
	}

	virtual int64 GetHash(const unsigned char* buffer, size_t len) 
	{
		//int64 challenge = 0xd0e1a1d3b4a9b9e;
		int64 challenge = 0xd0ea9b9e;
		for(size_t i=0;i<len;++i)
			challenge = challenge * buffer[i] * 256 + buffer[i];
		return challenge;
	}

	virtual int64 GetHash(const char* str)
	{
		//int64 challenge = 0xd0e1a1d3b4a9b9e;
		int64 challenge = 0xd0ea9b9e;
		for(const char* p=str; *p; ++p)
			challenge = challenge * (*p) * 256 + (*p);
		return challenge;
	}

	virtual int64 GetHash(int64 passhash, int64 challenge)
	{
		return (passhash*challenge) + (passhash+challenge) + (passhash^challenge);
	}

	virtual int64 RequestPassWord(const char* account, int64 challenge, bool force)
	{
		PassMap::iterator pi=m_passmap.find(account);
		if(pi!=m_passmap.end())
		{
			return pi->second;
		}
		else
		{
			return 0;
		}
	}

	virtual int64 GetPassWordHash(const char* account, int64 challenge)
	{
		PassMap::iterator pi=m_passmap.find(account);
		if(pi!=m_passmap.end())
		{
			return pi->second;
		}
		else
		{
			return 0;
		}
	}

	virtual int64 GenerateRandomNumber()
	{
		return m_count++;
	}

	virtual void  GeneratePuzzle(client_puzzle_t* p, int difficulty)
	{
		p->answer=101;
		p->startp=0;
		p->endp=200;
		p->hash_val[0] = 123;
		p->hash_val[3] = 124;
	}
	virtual int   SolvePuzzle(client_puzzle_t* p, int64* result)//return 1:ok, 0:continue, -1:fail
	{
		*result = 101;
		return 1;
		for(int i=0;i<50;++i,++p->startp)
		{
			if(p->startp > p->endp)
				return -1;
			if(p->hash_val[0]==123 && p->hash_val[3]==124 && p->startp==101)
			{
				*result = p->startp;
				return 1;
			}
		}
		return 0;
	}

	virtual void  Encrypt(const BYTE *plainText, BYTE *cipherText, size_t len, int64 key)
	{
		int64* c=(int64*)cipherText;
		int64* p=(int64*)plainText;
		len>>=3;
		for(size_t i=0;i<len;++i)
		{
			c[i] = p[i]^key;
		}
	}
	virtual void  Decrypt(const BYTE *cipherText, BYTE *plainText, size_t len, int64 key)
	{
		int64* c=(int64*)cipherText;
		int64* p=(int64*)plainText;
		len>>=3;
		for(size_t i=0;i<len;++i)
		{
			p[i] = c[i]^key;
		}
	}
	virtual void AddAccount(const char* acc, int64 passwd_hash, tick_t time_out)
	{
		m_passmap[acc] = passwd_hash;
	}

	std::set<int64> m_passes;
	virtual void  AddDirectPassword(int64 pass, tick_t time_out)
	{
		m_passes.insert(pass);
	}
	virtual void  RemoveDirectPassword(int64 pass)
	{
		m_passes.erase(pass);
	}
	virtual int64 FindDirectPassword(int64 pass)
	{
		if(m_passes.find(pass) == m_passes.end())
			return 0;
		else
			return pass;
	}
	virtual void UpdateAuthentication()
	{
	}
	virtual void SetPasswdDB(IPasswdDB* db)
	{
	}
};

#endif
