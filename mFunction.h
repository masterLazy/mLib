#pragma once
/*****************************************************************************
* mFunction.h
* 函数集
*
*****************************************************************************/
typedef float Func_F;

#include <Windows.h>
#include <windowsx.h>	//更多宏
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
#include <stdarg.h>
#include <io.h>
#include <iostream>
#include <iomanip>
#include <thread>

#ifdef max
#undef max
template<typename T> inline T max(T a, T b)
{
	return a > b ? a : b;
}
#endif
#ifdef min
#undef min
template<typename T> inline T min(T a, T b)
{
	return a < b ? a : b;
}
#endif

namespace mlib
{
	/*****************************************************************************
	* 渐变
	*****************************************************************************/

	//线性渐变
	inline Func_F Linear(Func_F start, Func_F end, Func_F rate)
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
	inline Func_F Smooth(Func_F start, Func_F end, Func_F rate, int mode = 2)
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
	*****************************************************************************/

	//控制台输出Unicode字符
	inline void printU(const std::wstring& wstr)
	{
		WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), wstr.c_str(), wstr.size(), NULL, NULL);
	}

	//输出进度条
	enum class PBStyle
	{
		def,	//默认,	[###############...............]
		arrow,	//箭头,	[==============>...............]
		block	//方块,	|▉▉▉▉▉▉▉▉▉▉▉▉▉▉▌...............|
				/*使用的符号
				* █,1
				* ▉,7/8
				* ▊,3/4
				* ▋,5/8
				* ▌,1/2
				* ▍,3/8
				* ▎,1/4
				* ▏,1/8
				*/
	};
	inline void printPB(Func_F rate, PBStyle style = PBStyle::def)
	{
		using namespace std;

		int cnt = round(rate * 30);

		switch (style)
		{
		case PBStyle::def:
			cout << "[";
			for (int i = 0; i < cnt; i++)
			{
				cout << "#";
			}
			for (int i = 0; i < 30 - cnt; i++)
			{
				cout << ".";
			}
			cout << "]";
			break;

		case PBStyle::arrow:
			cout << "[";
			for (int i = 0; i < cnt; i++)
			{
				if (i == cnt - 1 && cnt != 30)cout << ">";
				else cout << "=";
			}
			for (int i = 0; i < 30 - cnt; i++)
			{
				cout << ".";
			}
			cout << "]";
			break;

		case PBStyle::block://有Bug!!!
			cout << "|";
			cnt = rate * 30;//这个比较特殊
			for (int i = 0; i < cnt; i++)
			{
				if (i == cnt - 1 && cnt != 30)
				{
					Func_F rem = rate - (float)cnt / 30;
					switch (int(rem * 8 * 30))
					{
					case 0:printU(L"."); break;
					case 1:printU(L"▏"); break;
					case 2:printU(L"▎"); break;
					case 3:printU(L"▍"); break;
					case 4:printU(L"▌"); break;
					case 5:printU(L"▋"); break;
					case 6:printU(L"▊"); break;
					case 7:printU(L"▉"); break;
					}
				}
				else printU(L"█");
			}
			for (int i = 0; i < 30 - cnt; i++)
			{
				cout << ".";
			}
			cout << "|";
			break;
		}
	}

	//输出文件大小
	inline void printSize(size_t size)
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
	inline void printLastError(DWORD lastError)
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

	//string<->wstring
	inline std::wstring StrToWstr(const std::string& str)
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
	inline std::string WstrToStr(const std::wstring& wstr)
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

	//系列击键消息
	inline void TypeEvent(const std::string& s)
	{
		for (auto ch : s)
		{
			if (ch >= 'A' && ch <= 'Z')
			{
				keybd_event(ch, 0, KEYEVENTF_EXTENDEDKEY, 0);
				Sleep(1);
				keybd_event(ch, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			}
			else if (ch >= 'a' && ch <= 'z')
			{
				ch = ch - 'a' + 'A';
				keybd_event(ch, 0, KEYEVENTF_EXTENDEDKEY, 0);
				Sleep(1);
				keybd_event(ch, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			}
		}
	}

	/*****************************************************************************
	* 色彩
	*****************************************************************************/

	//RGB->R,G,B
	template<typename T1> inline void SplitRgb(COLORREF rgb, T1* r, T1* g, T1* b)
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
	inline void RgbToHsv(COLORREF rgb, Func_F* h, Func_F* s, Func_F* v)
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
	inline COLORREF HsvToRgb(Func_F h, Func_F s, Func_F v)
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
	inline COLORREF RgbCC(COLORREF start, COLORREF end, Func_F rate)
	{
		int r1, g1, b1, r2, g2, b2;
		SplitRgb(start, &r1, &g1, &b1);
		SplitRgb(end, &r2, &g2, &b2);
		return RGB(Linear(r1, r2, rate), Linear(g1, g2, rate), Linear(b1, b2, rate));
	}

	//HSV渐变色
	inline COLORREF HsvCC(COLORREF start, COLORREF end, Func_F rate)
	{
		Func_F h1, s1, v1, h2, s2, v2;
		RgbToHsv(start, &h1, &s1, &v1);
		RgbToHsv(end, &h2, &s2, &v2);
		return HsvToRgb(Linear(h1, h2, rate), Linear(s1, s2, rate), Linear(v1, v2, rate));
	}

	//颜色映射
	enum class Cmap
	{
		viridis,	//翠绿色
		inferno,	//无法控制的大火
		gray,		//灰
		hsv			//HSV(蓝到红)
	};
	inline COLORREF ColorMap(Func_F rate, Cmap cmap = Cmap::viridis, Func_F min = 0, Func_F max = 1)
	{
		rate = (rate - min) / (max - min);//归一化
		switch (cmap)
		{
		case Cmap::viridis:
			return HsvToRgb(Linear(288, 54, rate),
				rate <= 0.2 ? Linear(100, 50, rate / 0.2) :
				rate <= 0.6 ? Linear(50, 80, (rate - 0.2) / 0.4) :
				rate <= 0.7 ? Linear(80, 50, (rate - 0.6) / 0.1) :
				Linear(50, 90, (rate - 0.7) / 0.3),
				Linear(30, 100, rate));

		case Cmap::inferno:
		{
			return HsvToRgb(
				rate <= 0.1 ? 240 :
				rate <= 0.6 ? Linear(240, 360, (rate - 0.1) / 0.5) :
				Linear(0, 60, (rate - 0.6) / 0.4),

				rate <= 0.2 ? 90 :
				rate <= 0.4 ? Linear(90, 75, (rate - 0.2) / 0.2) :
				rate <= 0.6 ? 75 :
				rate <= 0.7 ? Linear(75, 90, (rate - 0.6) / 0.1) :
				Linear(90, 100, (rate - 0.7) / 0.3),

				Linear(10, 100, rate));
		}

		case Cmap::gray:
			return RgbCC(RGB(0, 0, 0), RGB(255, 255, 255), rate);

		case Cmap::hsv:
			return HsvCC(RGB(0, 0, 255), RGB(255, 0, 0), rate);
		}
	}


	/*****************************************************************************
	* 剪贴板
	*****************************************************************************/

	//获取剪贴板内存的指针
	//uFormat: CF_xxx
	inline void* GetCbData(UINT uFormat, size_t* size = nullptr)
	{
		//打开剪贴板
		if (!OpenClipboard(NULL))return nullptr;

		//获取内存块指针
		HGLOBAL hMem = GetClipboardData(uFormat);
		if (hMem != NULL)
		{
			//获取大小
			if (size != nullptr)*size = GlobalSize(hMem);

			//获取Lock
			void* pt = GlobalLock(hMem);

			//释放Lock
			GlobalUnlock(hMem);

			return pt;
		}
		else
		{
			return nullptr;
		}

		//关闭剪贴板
		CloseClipboard();
	}

	//清空剪贴板
	inline bool ClearCb()
	{
		if (!OpenClipboard(NULL))return false;
		if (!EmptyClipboard())return false;
		if (!CloseClipboard())return false;
	}

	//获取剪贴板中的文本
	inline std::wstring GetCbText()
	{
		wchar_t* str = nullptr;
		str = (wchar_t*)GetCbData(CF_UNICODETEXT);
		if (str != nullptr)
		{
			return std::wstring(str);
		}
		else
		{
			return L"";
		}
	}

	/*****************************************************************************
	* Room
	* 界面“房间”
	*****************************************************************************/

	class Room
	{
	public:
		std::vector<HWND> hChild;

		//添加子窗口(可自动改字体)
		void add(HWND hWnd, HFONT hFont = NULL)
		{
			if (hFont != NULL)
			{
				SetWindowFont(hWnd, hFont, NULL);
			}
			hChild.push_back(hWnd);
		}

		//显示
		void show()
		{
			for (auto i : hChild)ShowWindow(i, SW_SHOW);
		}
		//隐藏
		void hide()
		{
			for (auto i : hChild)ShowWindow(i, SW_HIDE);
		}
	};


	/*****************************************************************************
	* 其他
	*****************************************************************************/

	//绝对延迟
	inline void Delay(Func_F ms)
	{
		if (ms == 0)return;

		static clock_t last = clock();
		while (clock() <= last + ms);
		last = clock();
	}

	//查找指定元素
	inline int find(const std::vector<char>& v, const std::string& s)
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
	template<typename T1>
	inline size_t find(const T1 arr[], size_t len, const T1 value, const size_t begin = 0)
	{
		for (int i = begin; i < len; i++)
		{
			if (arr[i] == value)return i;
		}
		return -1;
	}

	inline std::string substr(std::vector<char> v, size_t off, size_t count)
	{
		std::string res;
		for (size_t i = 0; i < count; i++)
		{
			res += v[off + i];
		}
		return res;
	}

	/*inline Func_F max(Func_F a, Func_F b)
	{
		return a > b ? a : b;
	}
	template<typename T>inline T max(T a, T b)
	{
		return a > b ? a : b;
	}
	inline Func_F min(Func_F a, Func_F b)
	{
		return a < b ? a : b;
	}
	template<typename T>inline T min(T a, T b)
	{
		return a < b ? a : b;
	}*/

	//查找含指定字符串的窗口
	static HWND _find_hWnd;
	static std::wstring _find_str;
	inline BOOL CALLBACK _FindWindowProc(HWND hWnd, LPARAM lParam)
	{
		wchar_t szText[128] = { 0 };
		GetWindowText(hWnd, szText, 128);
		if (std::wstring(szText).find(_find_str) != std::string::npos)
		{
			_find_hWnd = hWnd;
			return false;
		}
		else
		{
			return true;
		}
	}
#undef FindWindow
	//查找含指定字符串的窗口
	inline HWND FindWindow(std::wstring str)
	{
		_find_str = str;
		_find_hWnd = NULL;
		EnumWindows(_FindWindowProc, NULL);
		return _find_hWnd;
	}

	//获取控制台内的文本(原理是Ctrl+A,Enter)
	inline std::wstring GetConsoleText(HWND hWnd)
	{
		if (hWnd != NULL)
		{
			//置顶控制台(我的终极方案)
			HWND hOldTopWnd = GetTopWindow(hWnd);
			HWND hOldFrWnd = GetForegroundWindow();
			BringWindowToTop(hWnd);
			SetForegroundWindow(hWnd);
			//Sleep(1);

			//保存并清空剪贴板
			std::wstring ws = GetCbText();
			ClearCb();

			//发送按键消息:
			//Ctrl+A
			keybd_event(VK_CONTROL, 0, KEYEVENTF_EXTENDEDKEY, 0);
			keybd_event('A', 0, KEYEVENTF_EXTENDEDKEY, 0);
			Sleep(1);
			keybd_event('A', 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			Sleep(1);
			//Ctrl+A
			keybd_event(VK_CONTROL, 0, KEYEVENTF_EXTENDEDKEY, 0);
			keybd_event('A', 0, KEYEVENTF_EXTENDEDKEY, 0);
			Sleep(1);
			keybd_event('A', 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			Sleep(1);
			//Enter
			keybd_event(VK_RETURN, 0, KEYEVENTF_EXTENDEDKEY, 0);
			Sleep(1);
			keybd_event(VK_RETURN, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

			//获取剪贴板中的文本
			std::wstring temp = GetCbText();

			//还原剪贴板
			//SetCbText(ws);

			//还原置顶窗口
			BringWindowToTop(hOldTopWnd);
			SetForegroundWindow(hOldFrWnd);

			return temp;
		}
		else
		{
			return L"";
		}
	}
}