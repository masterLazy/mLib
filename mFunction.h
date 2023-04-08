#pragma once
/*****************************************************************************
* mFunction.h
* 函数集
*
*****************************************************************************/
typedef float Func_F;

#include <Windows.h>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
#include <stdarg.h>
#include <io.h>
#include <iostream>
#include <iomanip>

#undef max
#undef min

namespace mlib
{
	/*****************************************************************************
	* 渐变
	*
	*****************************************************************************/

	//线性渐变
	Func_F Linear(Func_F start, Func_F end, Func_F rate)
	{
		if (rate < 0)rate = 0;
		else if (rate > 1)rate = 1;
		return start + (end - start) * rate;
	}

	//平滑渐变
	//mode
	//0: 缓入
	//1: 缓出
	//2: 缓入+缓出
	Func_F Smooth(Func_F start, Func_F end, Func_F rate, int mode = 2)
	{
		if (rate < 0)rate = 0;
		else if (rate > 1)rate = 1;
		if (mode == 0 && rate > 0.5)
		{
			return start + (end - start) * rate;
		}
		if (mode == 1 && rate < 0.5)
		{
			return start + (end - start) * rate;
		}
		return start + (end - start) * (3 * (1 - rate) * powf(rate, 2) + powf(rate, 3));
	}

	/*****************************************************************************
	* 控制台/字符串
	*
	*****************************************************************************/

	//控制台输出Unicode字符
	void printU(const std::wstring& wstr)
	{
		WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), wstr.c_str(), wstr.size(), NULL, NULL);
	}

	//输出进度条
	void printPB(float rate)
	{
		using namespace std;
		//TensorFlow式的
		//形如[============================>.]
		int c = ceil(rate * 30);
		std::cout << "[";
		for (int i = 0; i < c; i++)
		{
			if (i == c - 1 && c != 30)cout << ">";
			else cout << "=";
		}
		for (int i = 0; i < 30 - c; i++)
		{
			cout << ".";
		}
		cout << "]";
	}

	//输出文件大小
	void printSize(size_t size)
	{
		using namespace std;
		if (size < 1024)
		{
			cout << size << " B";
		}
		else if (size < 1024 * 1024)
		{
			cout << fixed << setprecision(2) << float(size) / 1024 << " KB";
		}
		else if (size < 1024 * 1024 * 1024)
		{
			cout << fixed << setprecision(2) << float(size) / 1024 / 1024 << " MB";
		}
		else
		{
			cout << fixed << setprecision(2) << float(size) / 1024 / 1024 / 1024 << " GB";
		}
	}

	//输出GetLastError()错误代码对应的信息
	void printLastError(DWORD lastError)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			lastError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		printU((wchar_t*)lpMsgBuf);
	}

	//字符串距离
	class levenshtein
	{
	public:
		static int compare(const std::string& s1, const std::string& s2)
		{
			// create two work vectors of integer distances
			const int m = s1.size();
			const int n = s2.size();
			int* v1 = new int[n + 1];
			int* v2 = new int[n + 1];

			// initialize v1 (the previous row of distances)
			// this row is A[0][i]: edit distance for an empty s1
			// the distance is just the number of characters to delete from s2
			for (int i = 0; i <= n; ++i) {
				v1[i] = i;
			}

			// calculate v2 (current row distances) from the previous row v1
			for (int i = 0; i < m; ++i) {
				// first element of v2 is A[i+1][0]
				// edit distance is delete (i+1) chars from s to match empty s2
				v2[0] = i + 1;

				// use formula to fill in the rest of the row
				for (int j = 0; j < n; ++j) {
					// calculating costs for A[i+1][j+1]
					int deletionCost = v1[j + 1] + 1;
					int insertionCost = v2[j] + 1;
					int substitutionCost = v1[j];
					if (s1[i] != s2[j]) {
						++substitutionCost;
					}
					v2[j + 1] = min3(deletionCost, insertionCost, substitutionCost);
				}
				// copy v2 (current row) to v1 (previous row) for next iteration
				swap(v1, v2);
			}

			// after the last swap, the results of v2 are now in v1
			int retval = v1[n];
			delete[]v1;
			delete[]v2;
			return retval;
		}
	private:
		static int min3(int a, int b, int c)
		{
			if (a < b) {
				return a < c ? a : c;
			}
			else {
				return b < c ? b : c;
			}
		}
		static void swap(int*& a, int*& b)
		{
			int* tmp = a;
			a = b;
			b = tmp;
		}
	};
	//https://blog.csdn.net/tissar/article/details/86475879

	/*****************************************************************************
	* 其他
	*
	*****************************************************************************/

	//绝对延迟
	void Delay(Func_F ms)
	{
		if (ms == 0)return;

		static clock_t last = clock();
		while (clock() <= last + ms);
		last = clock();
	}

	//随机小数
	Func_F Random(Func_F min, Func_F max)
	{
		return min + (Func_F)rand() / RAND_MAX * (max - min);
	}

	//查找指定元素
	int find(const std::vector<char>& v, const std::string& s)
	{
		for (int i = 0; i < v.size() - s.size(); i++)
		{
			for (int j = 0; j < s.size(); j++)
			{
				if (v[i + j] != s[j])break;
				else if (j == s.size() - 1)return i;
			}
		}
		return std::string::npos;
	}
	template<typename T>
	inline size_t find(const T arr[], size_t len, const T value, const size_t begin = 0)
	{
		for (int i = begin; i < len; i++)
		{
			if (arr[i] == value)return i;
		}
		return -1;
	}

	std::string substr(std::vector<char> v, size_t off, size_t count)
	{
		std::string res;
		for (size_t i = 0; i < count; i++)
		{
			res += v[off + i];
		}
		return res;
	}

	//string<->wstring
	std::wstring StrToWstr(const std::string& str)
	{
		std::wstring result;
		//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
		int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
		TCHAR* buffer = new TCHAR[len + 1];
		//多字节编码转换成宽字节编码  
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
		buffer[len] = '\0';             //添加字符串结尾  
		//删除缓冲区并返回值  
		result.append(buffer);
		delete[] buffer;
		return result;
	}
	std::string WstrToStr(const std::wstring& wstr)
	{
		std::string result;
		//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
		int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
		char* buffer = new char[len + 1];
		//宽字节编码转换成多字节编码  
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
		buffer[len] = '\0';
		//删除缓冲区并返回值  
		result.append(buffer);
		delete[] buffer;
		return result;
	}
	//————————————————
	//版权声明：本文为CSDN博主「Panix2n_n」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
	//原文链接：https ://blog.csdn.net/mercy_ps/article/details/81218608


	/*****************************************************************************
	* 色彩
	*
	*****************************************************************************/

	//RGB->R,G,B
	template<typename T> void SplitRgb(COLORREF rgb, T* r, T* g, T* b)
	{
		*r = GetRValue(rgb);
		*g = GetGValue(rgb);
		*b = GetBValue(rgb);
	}

	/* r,g,b及h,s,v的取值
	*		参数和返回值		函数中
	* r		0~255			0~1
	* g		0~255			0~1
	* b		0~255			0~1
	*
	* h		0~360			0~1
	* s		0~100			0~1
	* v		0~100			0~1
	*/

	//RGB->H,S,V
	void RgbToHsv(COLORREF rgb, Func_F* h, Func_F* s, Func_F* v)
	{
		Func_F r, g, b, max = 0, min = 0;
		SplitRgb(rgb, &r, &g, &b);
		r /= 255;
		g /= 255;
		b /= 255;

		max = r > g ? r : g;
		max = max > b ? max : b;
		min = r < g ? r : g;
		min = min < b ? min : b;

		*v = max;
		if (max == 0) *s = 0;
		else *s = 1 - (min / max);

		if (max == min)
		{
			*h = 0;
		}
		else if (r == max && g >= b)
		{
			*h = 60 * ((g - b) / (max - min));
		}
		else if (r == max && g < b)
		{
			*h = 60 * ((g - b) / (max - min)) + 360;
		}
		else if (g == max)
		{
			*h = 60 * ((b - r) / (max - min)) + 120;
		}
		else if (b == max)
		{
			*h = 60 * ((r - g) / (max - min)) + 240;
		}
		*v = *v * 100;
		*s = *s * 100;
	}

	//H,S,V->RGB
	COLORREF HsvToRgb(Func_F h, Func_F s, Func_F v)
	{
		Func_F r, g, b, c = 0, x = 0, y = 0, z = 0;
		int i = 0;
		s /= 100;
		v /= 100;
		if (s == 0)
		{
			r = v;
			g = v;
			b = v;
		}
		else
		{
			h /= 60;
			i = h;
			c = h - i;

			x = v * (1 - s);
			y = v * (1 - s * c);
			z = v * (1 - s * (1 - c));
			switch (i)
			{
			case 0:
				r = v; g = z; b = x; break;

			case 1:
				r = y; g = v; b = x; break;

			case 2:
				r = x; g = v; b = z; break;

			case 3:
				r = x; g = y; b = v; break;

			case 4:
				r = z; g = x; b = v; break;

			case 5:
			default:
				r = v; g = x; b = y; break;
			}
		}
		return RGB(r * 255, g * 255, b * 255);
	}

	//渐变色
	COLORREF RgbCC(COLORREF start, COLORREF end, Func_F rate)
	{
		int r1, g1, b1, r2, g2, b2;
		SplitRgb(start, &r1, &g1, &b1);
		SplitRgb(end, &r2, &g2, &b2);
		return RGB(Linear(r1, r2, rate), Linear(g1, g2, rate), Linear(b1, b2, rate));
	}

	//HSV渐变色
	COLORREF HsvCC(COLORREF start, COLORREF end, Func_F rate)
	{
		Func_F h1, s1, v1, h2, s2, v2;
		RgbToHsv(start, &h1, &s1, &v1);
		RgbToHsv(end, &h2, &s2, &v2);
		return HsvToRgb(Linear(h1, h2, rate), Linear(s1, s2, rate), Linear(v1, v2, rate));
	}
}