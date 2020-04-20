#pragma once
#include "DataVerify.h"

#define MaxCovertStrLen 1023

inline std::string ItoA(int i)
{
    char buff[32];
    sprintf(buff, "%d", i);
    return buff;
}

inline std::string FtoA(float f)
{
    char buff[32];
    sprintf(buff, "%.2f", f);
    return buff;
}

inline std::wstring char2wchar(const char* cchar) 
{
    wchar_t wchar[1024];
    memset(wchar, 0, 1024 * sizeof(wchar_t));

    MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), wchar, 1024);
    return wchar;
}

inline std::string wchar2char(const wchar_t* wchar)
{
    char cchar[1024];
    memset(cchar, 0, 1024 * sizeof(char));

    WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), cchar, 1024, NULL, NULL);
    return cchar; 
}

inline int multibyteToUnicode(const char *multiByteStr, size_t multiByteStrLen, size_t multiByteCharsetFlag, char *unicodeStr, int unicodeStrLen)
{
    int len = -1;
    do 
    {
        if (multiByteStr == NULL || multiByteStrLen <= 0 || unicodeStr == NULL || unicodeStrLen <= 0)
            break;

        //ԭʼ�ַ����ַ�����ʶ�Ȳ���GBK/GB2312�ֲ���UTF-8
        if (multiByteCharsetFlag != CP_ACP && multiByteCharsetFlag != CP_UTF8)
            break;

        //�������������ȡת�������wchar_t�ĸ���
        int requiredUnicode = MultiByteToWideChar(multiByteCharsetFlag, 0, multiByteStr, multiByteStrLen, NULL, 0);
        //���ձ�ת���ַ����Ļ�����С��ʵ����Ҫ���ֽ���
        if((int)sizeof(WCHAR) * requiredUnicode > unicodeStrLen)
            break;

        {// �������ڲ���break��return
            WCHAR *tmp = new WCHAR[requiredUnicode];
            wmemset(tmp, 0, requiredUnicode);

            //ִ����ʵת��������ת�����unicode��д��tmp������
            int parsedUnicode = MultiByteToWideChar(multiByteCharsetFlag, 0, multiByteStr, multiByteStrLen, tmp, requiredUnicode);

            //�ж���ʵ������unicode�ַ����ͷ�����ַ����Ƿ�һ��
            if (parsedUnicode == requiredUnicode)
            {
                memset(unicodeStr, 0, unicodeStrLen);
                memcpy(unicodeStr, tmp, sizeof(WCHAR) * requiredUnicode);
                //���ؽ����õ����ֽ���
                len = sizeof(WCHAR) * requiredUnicode;
            }
            delete []tmp;
            tmp = NULL;
        }
    } while (0);
    return len;
}

inline int unicodeToMultibyte(const char *unicodeStr,size_t unicodeStrLen,char *multiByteStr,int multiByteStrLen,UINT multiByteCharsetFlag)
{
    int len = -1;
    do 
    {
        if (unicodeStr == NULL || unicodeStrLen <= 0 || multiByteStr == NULL || multiByteStrLen <= 0)
            break;

        //Ŀ���ַ�����ʹ�õ��ַ����Ȳ���CP_ACP��gbk/gb2312���ֲ���CP_UTF8��utf-8��
        if (multiByteCharsetFlag != CP_ACP && multiByteCharsetFlag != CP_UTF8)
            break;

        {// �������ڲ���break��return
            WCHAR *tmp = new WCHAR[unicodeStrLen / 2];
            wmemset(tmp, 0, unicodeStrLen / 2);
            memcpy(tmp, unicodeStr, unicodeStrLen);

            //��ȡ���ڴ��Ŀ���ַ������ֽ���
            int requiredByte = WideCharToMultiByte(multiByteCharsetFlag, 0, tmp, unicodeStrLen / 2, NULL, 0, NULL, NULL);

            //���ձ�ת���ַ����Ļ�����С��ʵ����Ҫ���ֽ���
            if (requiredByte <= multiByteStrLen)
            {
                memset(multiByteStr, 0, multiByteStrLen);
                //ִ������ת������ת����Ķ��ֽ��ַ�����ŵ�Ŀ�껺�����У�������ʵ�ʽ������ֽ���
                int parsedByte = WideCharToMultiByte(multiByteCharsetFlag, 0, tmp, unicodeStrLen / 2, multiByteStr, multiByteStrLen, NULL, NULL);
                if (parsedByte == requiredByte)
                {
                    len = parsedByte;//���ؽ����õ����ֽ���
                }
            }
            delete []tmp;
            tmp=NULL;
        }
    } while(0);
    return len;
}

inline std::string utf8ToGbk(const char * utf8str, size_t utf8strlen)
{
    std::string ret_str = "";
    do 
    {
        if (utf8str == NULL || utf8strlen <= 0)
            break;

        //���仺����������Ϊutf8�ַ������ȵ�2��
        char *unicode_str = new char[2 * (utf8strlen + 1)];
        memset(unicode_str, 0, 2 * (utf8strlen + 1));
        //utf8תunicode
        int unicodebytes = multibyteToUnicode(utf8str, utf8strlen, CP_UTF8, unicode_str, 2 * (utf8strlen + 1));
        if (unicodebytes >= 0)
        {
            //���仺����������Ϊutf8�ַ������ȵ�2��
            char *gbk_str = new char[2 * (utf8strlen + 1)];
            memset(gbk_str, 0, 2 * (utf8strlen + 1));
            //unicodeתGBK
            int gbkbytes = unicodeToMultibyte(unicode_str, unicodebytes, gbk_str, 2 * (utf8strlen + 1), CP_ACP);
            if (gbkbytes >= 0)
            {
                ret_str = gbk_str;
            }
            delete [] gbk_str;
            gbk_str = NULL;
        }
        delete [] unicode_str;
        unicode_str=NULL;
    } while (0);
    return ret_str;
}

inline std::string utf8ToGbk18030(const char * utf8str, size_t utf8strlen)
{
    std::string ret_str = "";
    ret_str = utf8ToGbk(utf8str, utf8strlen);
    return ret_str;
}

inline std::string gbkToUtf8(const char * gbkstr, size_t gbkstrlen)
{
    std::string ret_str = "";
    do 
    {
        if (gbkstr == NULL || gbkstrlen <= 0 || gbkstrlen > MaxCovertStrLen)
            break;

        //���仺����������Ϊutf8�ַ������ȵ�2��
        char unicode_str[2 * (MaxCovertStrLen + 1)];
        memset(unicode_str, 0, 2 * (MaxCovertStrLen + 1));
        //utf8תunicode
        int unicodebytes = multibyteToUnicode(gbkstr, gbkstrlen, CP_ACP, unicode_str, 2 * (gbkstrlen + 1));
        if (unicodebytes >= 0)
        {
            //���仺����������Ϊutf8�ַ������ȵ�2��
            char gbk_str[2 * (MaxCovertStrLen + 1)];
            memset(gbk_str, 0, 2 * (MaxCovertStrLen + 1));
            //unicodeתGBK
            int gbkbytes = unicodeToMultibyte(unicode_str, unicodebytes, gbk_str, 2 * (gbkstrlen + 1), CP_UTF8);
            if (gbkbytes >= 0)
            {
                ret_str = gbk_str;
            }
        }
    } while (0);
    return ret_str;
}

inline std::string gbk18030ToUtf8(const char * gbkstr, size_t gbkstrlen)
{
    std::string ret_str = "";
    ret_str = gbkToUtf8(gbkstr, gbkstrlen);
    return ret_str;
}

inline int bibo_snprintf(char *buffer, size_t count, const char *format, ...)
{
	va_list argptr;
	va_start (argptr, format);

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	int res =_vsnprintf(buffer, count, format, argptr);
#else
	int res = vsnprintf(buffer, count, format, argptr);
	if(res>=count)
		res=-1;
#endif

	va_end (argptr);
	return res;
}

inline std::string GenerateConfigMD5(const std::string& content)
{
	char md5_buf[16] = "";
	char md5_readable[33] = "";

	CDataVerify::GetMD5(md5_buf, content.c_str(), static_cast<int>(content.size()));
	CDataVerify::ConvertMD5toReadableString(md5_buf, md5_readable);
	return md5_readable;
}
