#include "FileManager.h"

#include <sys/types.h>
#include <sys/stat.h>
// Apparently have to do this for non windows I guess, haven't actually tested
#ifndef _WIN32
#include <unistd.h>
#define _stat stat 
#endif

#ifndef _WIN32
#endif

namespace file
{
	bool isNewer(const std::string &file0, const std::string &file1)
	{
		struct _stat file0Result, file1Result;
		if (!_stat(file0.c_str(), &file0Result))
		{
			auto mod_time0 = file0Result.st_mtime;

			if (!_stat(file1.c_str(), &file1Result))
			{
				auto mod_time1 = file1Result.st_mtime;

				if (mod_time0 < mod_time1)
				{
					return true;
				}
			}
			else
			{
				SAT_DEBUG_LOG_WARNING("%s cannot be found!", file1.c_str());
			}
		}
		else
		{
			SAT_DEBUG_LOG_WARNING("%s cannot be found!", file0.c_str());
		}
		return false;
	}

	bool fileExists(const std::string & filename)
	{
		struct _stat buffer;
		return (_stat(filename.c_str(), &buffer) == 0);
	}
}