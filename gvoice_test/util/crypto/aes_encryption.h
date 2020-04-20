/*********************************************************************
* Created:		2017/4/23 14:59:07
* File name:	\star\base_util\crypto\encryption.h
* Author:		刘彦鹏
* Revision:		1.0
*
* Copyright (C) 2010 - 2015 Horizon3D, All Rights Reserved.
*
* Purpose:		提供对称加解密算法
*********************************************************************/
#ifndef __ENCRYPTION_H_3944580956__
#define __ENCRYPTION_H_3944580956__
#include "../../share/netengine/BiboFrame/IAesEncryption.h"
#include <netengine/BiboFrame/BiboInterfaces.h>
#include "libtomcrypt/mycrypt.h"
#include <string>
class CAesEncryption : public IAesEncryption
{
public:
	CAesEncryption();
	~CAesEncryption();
public:
	virtual void Init() ;
	virtual void UnInit() ;
	virtual bool IsInit() ;



	//各种加解密算法
	virtual bool Encrypt(const char* key, const unsigned char *input, unsigned char *output) ;

	virtual bool Decrypt(const char* key, const unsigned char *input, unsigned char *output) ;

	// 使用AES对数据加密时需要提供的最小输出buffer的长度
	virtual int GetAesEncryptNeedOutBufSize(int inputSize);

	//加解密数据
	virtual bool Encrypt(const char* key, const unsigned char* input, int inputSize,
		unsigned char* output, int outBuffLen, int& outLen) ;

	//解密数据
	virtual bool Decrypt(const char* key, const unsigned char* input, int inputSize,
		unsigned char* output, int outputSize, int& outLen) ;


	virtual int Base64Encode(const unsigned char *in,  unsigned int len, 
		unsigned char *out, unsigned int *outlen);
 
	virtual int Base64Decode(const unsigned char *in,  unsigned int len, 
		unsigned char *out, unsigned int *outlen);
protected:
	bool m_init;
};
#endif//__ENCRYPTION_H_3944580956__

