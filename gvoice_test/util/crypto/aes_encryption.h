/*********************************************************************
* Created:		2017/4/23 14:59:07
* File name:	\star\base_util\crypto\encryption.h
* Author:		������
* Revision:		1.0
*
* Copyright (C) 2010 - 2015 Horizon3D, All Rights Reserved.
*
* Purpose:		�ṩ�ԳƼӽ����㷨
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



	//���ּӽ����㷨
	virtual bool Encrypt(const char* key, const unsigned char *input, unsigned char *output) ;

	virtual bool Decrypt(const char* key, const unsigned char *input, unsigned char *output) ;

	// ʹ��AES�����ݼ���ʱ��Ҫ�ṩ����С���buffer�ĳ���
	virtual int GetAesEncryptNeedOutBufSize(int inputSize);

	//�ӽ�������
	virtual bool Encrypt(const char* key, const unsigned char* input, int inputSize,
		unsigned char* output, int outBuffLen, int& outLen) ;

	//��������
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

