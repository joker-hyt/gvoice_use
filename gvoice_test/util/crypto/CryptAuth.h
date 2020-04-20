#ifndef __CRYPT_AUTH_H__gtu3ionhhi0hffr__
#define __CRYPT_AUTH_H__gtu3ionhhi0hffr__

#include "CryptMath.h"
#include "../net/GameConnection.h"
#include <string>
#include <map>
#include <set>

class CCryptAuth : public IAuthentication
{
protected:
	struct pass_entry_t
	{
		int64 pass;
		tick_t start_time;
		tick_t time_out;
	};

	typedef std::map<std::string, pass_entry_t> AccountMap;
	AccountMap m_accmap;
	IPasswdDB* m_passdb;

	CBiboLock m_lock;

	typedef std::map<int64, pass_entry_t> DPassMap;
	DPassMap m_passes;

	tick_t m_last_check;
public:
	CCryptAuth()
	{
		m_passdb = 0;
		m_last_check = 0;
	}
	virtual ~CCryptAuth()
	{
	}
	virtual void SetPasswdDB(IPasswdDB* db)
	{
		m_passdb = db;
	}
	virtual void Release()
	{
		delete this;
	}

	virtual void GetMD5Str(char* md5str, const char* str);

	virtual int64 GetHash(const unsigned char* buffer, size_t len);
	virtual int64 GetHash(const char* str);
	virtual int64 GetHash(int64 passhash, int64 challenge);
	virtual int64 RequestPassWord(const char* account, int64 challenge, bool force);
	virtual int64 GetPassWordHash(const char* account, int64 challenge);
	virtual int64 GenerateRandomNumber();
	virtual void  GeneratePuzzle(client_puzzle_t* p, int difficulty);
	virtual int   SolvePuzzle(client_puzzle_t* p, int64* result);//return 1:ok, 0:continue, -1:fail
	virtual void  Encrypt(const BYTE *plainText, BYTE *cipherText, size_t len, int64 key);
	virtual void  Decrypt(const BYTE *cipherText, BYTE *plainText, size_t len, int64 key);

	virtual void  AddDirectPassword(int64 pass, tick_t time_out);
	virtual void  RemoveDirectPassword(int64 pass);
	virtual int64 FindDirectPassword(int64 pass);
	void CheckTimeOut();
	virtual void AddAccount(const char* str, int64 passwd_hash, tick_t time_out); //-1±íÊ¾½ûÖ¹
	virtual void UpdateAuthentication();

	void SnapShot(int lv,unsigned int verbose)const
	{
		TakeSnapShot(lv,verbose, m_accmap, 0);
	}
};

#endif
