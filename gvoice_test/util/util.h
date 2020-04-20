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

        //原始字符串字符集标识既不是GBK/GB2312又不是UTF-8
        if (multiByteCharsetFlag != CP_ACP && multiByteCharsetFlag != CP_UTF8)
            break;

        //此种情况用来获取转换所需的wchar_t的个数
        int requiredUnicode = MultiByteToWideChar(multiByteCharsetFlag, 0, multiByteStr, multiByteStrLen, NULL, 0);
        //接收被转换字符串的缓冲区小于实际需要的字节数
        if((int)sizeof(WCHAR) * requiredUnicode > unicodeStrLen)
            break;

        {// 此区间内不能break或return
            WCHAR *tmp = new WCHAR[requiredUnicode];
            wmemset(tmp, 0, requiredUnicode);

            //执行真实转换，并将转换后的unicode串写到tmp缓存区
            int parsedUnicode = MultiByteToWideChar(multiByteCharsetFlag, 0, multiByteStr, multiByteStrLen, tmp, requiredUnicode);

            //判断真实解析的unicode字符数和分配的字符数是否一致
            if (parsedUnicode == requiredUnicode)
            {
                memset(unicodeStr, 0, unicodeStrLen);
                memcpy(unicodeStr, tmp, sizeof(WCHAR) * requiredUnicode);
                //返回解析好的总字节数
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

        //目标字符串所使用的字符集既不是CP_ACP（gbk/gb2312）又不是CP_UTF8（utf-8）
        if (multiByteCharsetFlag != CP_ACP && multiByteCharsetFlag != CP_UTF8)
            break;

        {// 此区间内不能break或return
            WCHAR *tmp = new WCHAR[unicodeStrLen / 2];
            wmemset(tmp, 0, unicodeStrLen / 2);
            memcpy(tmp, unicodeStr, unicodeStrLen);

            //获取用于存放目标字符串的字节数
            int requiredByte = WideCharToMultiByte(multiByteCharsetFlag, 0, tmp, unicodeStrLen / 2, NULL, 0, NULL, NULL);

            //接收被转换字符串的缓冲区小于实际需要的字节数
            if (requiredByte <= multiByteStrLen)
            {
                memset(multiByteStr, 0, multiByteStrLen);
                //执行真正转换，将转换后的多字节字符串存放到目标缓存区中，并返回实际解析的字节数
                int parsedByte = WideCharToMultiByte(multiByteCharsetFlag, 0, tmp, unicodeStrLen / 2, multiByteStr, multiByteStrLen, NULL, NULL);
                if (parsedByte == requiredByte)
                {
                    len = parsedByte;//返回解析好的总字节数
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

        //分配缓存区，长度为utf8字符串长度的2倍
        char *unicode_str = new char[2 * (utf8strlen + 1)];
        memset(unicode_str, 0, 2 * (utf8strlen + 1));
        //utf8转unicode
        int unicodebytes = multibyteToUnicode(utf8str, utf8strlen, CP_UTF8, unicode_str, 2 * (utf8strlen + 1));
        if (unicodebytes >= 0)
        {
            //分配缓存区，长度为utf8字符串长度的2倍
            char *gbk_str = new char[2 * (utf8strlen + 1)];
            memset(gbk_str, 0, 2 * (utf8strlen + 1));
            //unicode转GBK
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

        //分配缓存区，长度为utf8字符串长度的2倍
        char unicode_str[2 * (MaxCovertStrLen + 1)];
        memset(unicode_str, 0, 2 * (MaxCovertStrLen + 1));
        //utf8转unicode
        int unicodebytes = multibyteToUnicode(gbkstr, gbkstrlen, CP_ACP, unicode_str, 2 * (gbkstrlen + 1));
        if (unicodebytes >= 0)
        {
            //分配缓存区，长度为utf8字符串长度的2倍
            char gbk_str[2 * (MaxCovertStrLen + 1)];
            memset(gbk_str, 0, 2 * (MaxCovertStrLen + 1));
            //unicode转GBK
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
