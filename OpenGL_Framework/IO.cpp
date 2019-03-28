#include "IO.h"
#include <gmtk/gmtk.h>
#include "AniMath.h"
#include <SatMath/sys/IgnoreWarning.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "FileManager.h"

namespace io
{
IGNORE_WARNING_PUSH
IGNORE_WARNING_NAMELESS_UNION_STRUCT
	struct stringCol
	{
		enum console
		{
			black = 0,
			blueDark,
			greenDark,
			cyanDark,
			redDark,
			purpleDark,
			yellowDark,
			whiteDark,
			gray,
			blue,
			green,
			cyan,
			red,
			purple,
			yellow,
			white
		};

		stringCol() = default;
		stringCol(const std::string &input, BYTE foreColor = whiteDark, BYTE backColor = black)
		{
			str = input;
			colBack = backColor;
			colFore = foreColor;
		}

		std::string str;
		BYTE colBack = black;
		BYTE colFore = whiteDark;

		void print()
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colBack * 16 + colFore);
			printf("%s", str.c_str());
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), whiteDark);
		}
	};
IGNORE_WARNING_POP

	std::string readFile(const std::string &filename)
	{
		std::ifstream inStream(filename);
		if (!inStream.good())
		{
			std::cout << "Shader File not found:\"" << filename << "\"\n";
			return std::string();
		}
		std::string data(std::istreambuf_iterator<char>(inStream), (std::istreambuf_iterator<char>()));
		return data;
	}

	void removeChar(std::string &str, char remove)
	{
		str.erase(std::remove(str.begin(), str.end(), remove), str.end());
	}

	//removes any of the characters in "str" that are in "remove"
	void removeChar(std::string &str, const std::string &remove)
	{
		for (size_t i = str.size() - 1; i != std::string::npos; --i)
			for (size_t c = 0; c < remove.size(); ++c)
				if (str[i] == remove[c])
					str.erase(str.begin() + i);
	}

	std::string addInclude(std::string data, const std::string & originalDirectory)
	{
		// Do this for every include statement in the file
		size_t includeLoc;
		while ((includeLoc = data.find("#include")) != std::string::npos)
		{
			size_t endlineLoc = data.find('\n', includeLoc);
			std::string includePath = data.substr(includeLoc + 8, endlineLoc - includeLoc - 8);
			// Find end of line, 
			removeChar(includePath, " \t\"");

			size_t lastDashLoc = originalDirectory.find_last_of("/\\");
			std::string directory = originalDirectory.substr(0, lastDashLoc+1) + includePath;

			if (file::fileExists(directory))
			{
				// Leave include signature
				std::string include = readFile(directory);
				include = "// [INCLUDE \"" + includePath + "\"]\n" + include + "\n// [END INCLUDE]\n";
				// replace #include line with the file loaded from the include
				data.replace(includeLoc, endlineLoc - includeLoc, include);
			}
			else
			{
				SAT_ERROR_LOC("addInclude()\n%s does not exist!", directory.c_str());
				data.erase(includeLoc, endlineLoc - includeLoc);
			}

		}

		return data;
	}

	std::string zeroPadNumber(int num, unsigned int padding)
	{
		return padNumber(num, padding, '0');
	}

	std::string padNumber(int num, unsigned int padding, char padChar)
	{
		std::string ret = std::to_string(num);

		// Append zero chars
		unsigned int str_length = ret.length();
		for (unsigned int i = 0; i < padding - str_length; i++)
			ret = padChar + ret;
		return ret;
	}	

	stringCol chooseLineColor(stringCol &str, int lineNum)
	{
		if (str.str[0] == '/' && str.str[1] == '/')
		{
			str.colFore = stringCol::greenDark;
			return str;
		}
		// TODO: Add functionality for /* & */ to work
		// Would need to rework this mess of a system

		//*
		// These were tests for trying different line colors for each row
		// It didn't look that nice in the end
		//if (lineNum % 4 >= 2)
		//{
		//	str.colBack = stringCol::blueDark;
		//}
		//else
		//{
		//	str.colBack = stringCol::black;
		//}
		if ((lineNum-1) % 10 < 5)
		{
			str.colFore = stringCol::whiteDark;
		}
		else
		{
			str.colFore = stringCol::gray;
		}
		//*/
		//str.colFore = stringCol::gray;
		return str;
	}

	stringCol createLineNumberStr(int lineNum, int padding)
	{
		stringCol foo;
		foo.str = padNumber(lineNum, padding, ' ') + '\t'; // pad to however many digits there are for the last line number
		chooseLineColor(foo, lineNum);
		return foo;
	}

	void printWithLineNumbers(const std::string & data)
	{
		size_t padding = log10Int(std::count(data.begin(), data.end(), '\n')) + 1;
		size_t lineNumber = 1;
		size_t strLoc = 0;
		size_t endLoc = 0;
		std::vector<stringCol> strings;
		while (strLoc < data.size())
		{
			strings.push_back(createLineNumberStr(lineNumber, padding));
			endLoc = data.find_first_of('\n', strLoc);
			
			stringCol lineStr = stringCol(data.substr(strLoc, endLoc - strLoc + 1));
			chooseLineColor(lineStr, lineNumber);
			strings.push_back(lineStr);

			++lineNumber;
			strLoc = endLoc + 1;
			if (endLoc == std::string::npos)
				break;
		}
		strings.push_back(stringCol("\n"));
		for (stringCol output : strings)
		{
			output.print();
		}
	}

	char easyToLower(char in) 
	{
		if (in <= 'Z' && in >= 'A')
			return in - ('Z' - 'z');
		return in;
	}

	std::string toLower(const std::string & input)
	{
		std::string data = input;
		std::transform(data.begin(), data.end(), data.begin(), easyToLower);
		return data;
	}

}

void setOpenGLName(GLenum identifier, GLuint name, const std::string & label)
{
	std::string labelEdit = label + " (" + std::to_string(name) + ")";
	glObjectLabel(identifier, name, static_cast<GLsizei>(labelEdit.size()), labelEdit.c_str());
}