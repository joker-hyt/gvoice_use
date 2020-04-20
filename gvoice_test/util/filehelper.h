#pragma once

#include<string>
#include<list>
#include<sys/stat.h>
#include <cstdlib>

#if !defined(_WIN32) && !defined(__WIN32__) && !defined(WIN32)
#include<fcntl.h>
#include<dirent.h>
#else
#include <io.h>
#include <direct.h>
#endif

class FileHelper
{
public:
	static bool fh_is_dir_exists(const std::string & dir_name);
	static bool fh_mk_dir(const std::string& dir_name);
	static bool fh_rm_file_dir(const std::string& dir_name);
	static bool fh_get_all_file_names(const std::string& dir_name, std::list<std::string>& files, bool recursive = false, const std::string& subfix = "");
	static bool fh_get_all_dir_names(const std::string& dir_name, std::list<std::string>& dirs, bool recursive = false); 
	static bool fh_get_last_mod_time(const std::string& file_name, int& last_mod_time);
	static bool fp_rename(const std::string& old_name, const std::string& new_name);
	static std::string GenerateConfigMD5(const std::string& content);
};

