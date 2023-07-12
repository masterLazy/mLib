#pragma once
/*****************************************************************************
* mCode.h
* 编解码
*
*****************************************************************************/

#include <string>
#include <codecvt>
#include <array>
#include "../mLib/mFunction.h"

namespace mlib
{
	/*****************************************************************************
	* Base64
	*****************************************************************************/

	const char base64[] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
		'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
		'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
		'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
	};

	std::string Base64_encode(const std::string& s, size_t len)
	{
		if (len == 0)return "";

		std::string res, temp(4, '\0');
		typedef unsigned char BYTE;
		BYTE temp2[4] = { 0 };
		for (int i = 0; i < len; i += 3)
		{
			//手动分组
			temp2[0] = BYTE(BYTE(s[i]) >> 2);

			if (i + 1 < len)temp2[1] = BYTE(BYTE(BYTE(s[i]) << 6) >> 2) + BYTE(BYTE(s[i + 1]) >> 4);
			else temp2[1] = BYTE(BYTE(BYTE(s[i]) << 6) >> 2);

			if (i + 2 < len)temp2[2] = BYTE(BYTE(BYTE(s[i + 1]) << 4) >> 2) + BYTE(BYTE(s[i + 2]) >> 6);
			else if (i + 1 < len)temp2[2] = BYTE(BYTE(BYTE(s[i + 1]) << 4) >> 2);

			if (i + 2 < len)temp2[3] = BYTE(BYTE(s[i + 2]) << 2) >> 2;
			//索引
			for (int i = 0; i < 4; i++)
			{
				temp[i] = base64[temp2[i]];
			}
			//填充"="
			if (i + 3 >= len && len % 3 == 1)temp[2] = temp[3] = '=';
			if (i + 3 >= len && len % 3 == 2)temp[3] = '=';

			res += temp;
		}
		return res;
	}
	std::string Base64_decode(const std::string& s, size_t* len = nullptr, bool* successed = nullptr)
	{
		if (s.size() % 4 != 0)return "";

		std::string res, temp(4, '\0');
		typedef unsigned char BYTE;
		BYTE temp2[3] = { 0 };
		if (len != nullptr)*len = 0;
		for (int i = 0; i < s.size(); i += 4)
		{
			//反索引
			for (int j = 0; j < 4; j++)
			{
				if (s[i + j] == '=')
				{
					temp[j] = 0;
					continue;
				}
				temp[j] = find(base64, 64, s[i + j]);
				if (temp[j] == -1)
				{
					if (successed != nullptr)*successed = false;
					return "";
				}
			}
			//手动分组
			temp2[0] = BYTE(BYTE(temp[0]) << 2) + BYTE(BYTE(temp[1]) >> 4);
			temp2[1] = BYTE(BYTE(temp[1]) << 4) + BYTE(BYTE(temp[2]) >> 2);
			temp2[2] = BYTE(BYTE(temp[2]) << 6) + BYTE(temp[3]);

			for (int j = 0; j < 3; j++)res += temp2[j];
			if (len != nullptr)*len += 3;
		}
		if (successed != nullptr)*successed = true;
		return res;
	}

	/*****************************************************************************
	* URI
	*****************************************************************************/

	std::string Uri_encode(const std::string& _s)
	{
		std::string s = _s;
		char temp, cstr[3];
		for (size_t i = 0; i < s.size(); i++)
		{
			if (!(s[i] >= 'A' && s[i] <= 'Z' ||
				s[i] >= 'a' && s[i] <= 'z' ||
				s[i] >= '0' && s[i] <= '9' ||
				s[i] == '-' ||
				s[i] == '_' ||
				s[i] == '.' ||
				s[i] == '~'))
			{
				temp = s[i];
				memset(cstr, 0, sizeof(cstr));
				_itoa_s(temp, cstr, 16);
				if (temp < 16)
				{
					cstr[1] = cstr[0];
					cstr[0] = '0';
				}
				_strupr_s(cstr);
				s[i] = '%';
				s.insert(i + 1, cstr, 2);
				i += 2;
			}
		}
		return s;
	}
	std::string Uri_decode(const std::string& _s)
	{
		std::string s = _s;
		int where;
		char temp;
		while (s.find("%") != std::string::npos)
		{
			where = s.find("%");
			temp = strtoul(s.substr(where + 1, 2).c_str(), NULL, 16);
			s.erase(where + 1, 2);
			s[where] = temp;
		}
		return s;
	}

	/*****************************************************************************
	* UTF-16 LE
	*****************************************************************************/

	std::wstring Utf16Le_decode(const std::string& s)
	{
		std::wstring res;
		for (size_t i = 0; i < s.size() / 2; i++)
		{
			res[i] = s[i * 2] + s[i * 2 + 1] << 4;
		}
		return res;
	}

	/*****************************************************************************
	* UTF-8
	*****************************************************************************/

	/*std::string Utf8_encode(const std::wstring& ws)
	{
		using namespace std;

		return wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(ws);
	}
	std::wstring Utf8_decode(const std::string& s)
	{
		using namespace std;

		wstring_convert<codecvt_utf8<wchar_t>> c;
		return c.from_bytes(s);
	}*/
}