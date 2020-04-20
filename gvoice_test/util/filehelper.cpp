#include"filehelper.h"

std::string FileHelper::GenerateConfigMD5(const std::string& content)
{
	char md5_buf[16] = "";
	char md5_readable[33] = "";
	IDataVerify* verify = GetBiboRegistry()->GetDataVerify();
	if(!verify)
	{
		LogError(__FUNCTION__, " get data verify failed");
		return false;
	}

	verify->GetMD5(md5_buf, content.c_str(), static_cast<int>(content.size()));
	verify->ConvertMD5toReadableString(md5_buf, md5_readable);
	return md5_readable;
}