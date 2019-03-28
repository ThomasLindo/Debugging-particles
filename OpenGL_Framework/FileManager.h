#pragma once

#include "IO.h"

/* 
  /////////////////
 // FileManager // 2019 Stephen Thompson
/////////////////

	Checks whether a file is newer or older than another file
	For example, this could be handy  checking whether a binary 
	mesh file needs to be recalculated


*/

namespace file
{
	bool isNewer(const std::string &file0, const std::string &file1);

	bool fileExists(const std::string &filename);
}