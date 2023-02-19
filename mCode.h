#pragma once
/******************************************************
* mCode.h
* 编解码
*
******************************************************/

#include <string>
#include <codecvt>
#include <array>
using namespace std;

/******************************************************
* Base64
*
******************************************************/

const array<const char, 64> base64 = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
	'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
	'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
	'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

//将string表示的二进制转换为char
char _ToChar(string str, int bit)
{
	char ans = 0;
	for (int i = 0; i < bit; i++)
	{
		if (str[i] == '1')ans += pow(2, bit - i - 1);
	}
	return ans;
}

int Base64_encode(string& res, const char str[], int length)
{
	res.clear();
	string temp;
	char szTemp[33], szTemp2[9], ch;//szTemp要33个bit才够装得下_itoa()的数据
	int iSize;
	for (int i = 0; i < length; i += 3)
	{
		temp.clear();
		//转为二进制字符串并拼成一组
		for (int j = 0; j < 3; j++)
		{
			if (i + j < length)_itoa_s(str[i + j], szTemp, 2);
			else memset(szTemp, '\0', sizeof(szTemp));
			//把szTemp的末8个char放到szTemp2里面去
			iSize = strlen(szTemp);
			memset(szTemp2, '0', sizeof(szTemp2));
			for (int k = iSize - 8; k < iSize; k++)
			{
				if (k < 0)continue;
				szTemp2[k - iSize + 8] = szTemp[k];
			}
			szTemp2[8] = '\0';
			temp += szTemp2;
		}
		//每6bit一组,分成4组并转为base64字符
		for (int j = 0; j < 4; j++)
		{
			ch = _ToChar(temp.substr(6 * j, 6), 6);
			if (i + j - 1 < length)res += base64[ch];
			else res += '=';
		}
	}
	return 0;
}
//解码最小4字节
int Base64_decode(string& res, string& str, int* length = nullptr)
{
	res.clear();
	string temp;
	char szTemp[7], ch;
	for (int i = 0; i < str.size(); i += 4)
	{
		temp.clear();
		//将每个base64字符转为6个bit的二进制字符串并拼在一起
		for (int j = 0; j < 4; j++)
		{
			if (i + j > str.size())
			{
				return -1;
			}
			
			auto res = find(base64.begin(), base64.end(), str[i + j]);
			if (str[i + j] != '=' && res != base64.end())
			{
				_itoa_s(distance(base64.begin(), res), szTemp, 2);
			}
			else memset(szTemp, '\0', sizeof(szTemp));
			for (int k = 0; k < 6 - strlen(szTemp); k++)temp += '0';
			temp += szTemp;
		}
		//每8个bit一组,分成3组并转为char
		if (length != NULL)*length = 0;
		for (int j = 0; j < 3; j++)
		{
			ch = _ToChar(temp.substr(8 * j, 8), 8);
			if (j + 1 > str.size())
			{
				return -1;
			}
			if (j + 1 < str.size() && str[j + 1] != '=')
			{
				res += ch;
				if (length != NULL)(*length)++;
			}
		}
	}
	return 0;
}

/******************************************************
* URI
*
******************************************************/

string Uri_encode(string str)
{
	char temp, cstr[3];
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!(str[i] >= 'A' && str[i] <= 'Z' ||
			str[i] >= 'a' && str[i] <= 'z' ||
			str[i] >= '0' && str[i] <= '9' ||
			str[i] == '-' ||
			str[i] == '_' ||
			str[i] == '.' ||
			str[i] == '~'))
		{
			temp = str[i];
			memset(cstr, 0, sizeof(cstr));
			_itoa_s(temp, cstr, 16);
			if (temp < 16)
			{
				cstr[1] = cstr[0];
				cstr[0] = '0';
			}
			_strupr_s(cstr);
			str[i] = '%';
			str.insert(i + 1, cstr, 2);
			i += 2;
		}
	}
	return str;
}
string Uri_decode(string str)
{
	int where;
	char temp;
	while (str.find("%") != string::npos)
	{
		where = str.find("%");
		temp = strtoul(str.substr(where + 1, 2).c_str(), NULL, 16);
		str.erase(where + 1, 2);
		str[where] = temp;
	}
	return str;
}

/******************************************************
* UTF-8
*
******************************************************/

string Utf8_encode(const wstring& ws)
{
	return wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(ws);
}
wstring Utf8_decode(const string& s)
{
	wstring_convert<codecvt_utf8<wchar_t>> c;
	return c.from_bytes(s);
}