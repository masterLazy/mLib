#pragma once
/******************************************************
* mFunction.h
* 函数集
*
******************************************************/
typedef float Func_F;

#include <Windows.h>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
#include <stdarg.h>
#include <io.h>

#ifdef _AMP_H
#include <amp_math.h>
#endif

/******************************************************
* 渐变
*
******************************************************/

//线性渐变
Func_F LnrC(Func_F start, Func_F end, Func_F rate)
{
	if (rate < 0)rate = 0;
	else if (rate > 1)rate = 1;
	return start + (end - start) * rate;
}
#ifdef _AMP_H
Func_F LnrC(Func_F start, Func_F end, Func_F rate) restrict(amp)
{
	if (rate < 0)rate = 0;
	else if (rate > 1)rate = 1;
	return start + (end - start) * rate;
}
#endif

//平滑渐变
//mode
//0: 缓入
//1: 缓出
//2: 缓入+缓出
Func_F SmoC(Func_F start, Func_F end, Func_F rate, int mode = 2)
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

/******************************************************
* 其他
*
******************************************************/

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

int find(std::vector<char> v, std::string s)
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
std::string substr(std::vector<char> v, size_t off, size_t count)
{
	std::string res;
	for (size_t i = 0; i < count; i++)
	{
		res += v[off + i];
	}
	return res;
}

//获取所有文件
void GetAllFiles(std::string path, std::vector<std::string>& fileDirs, std::vector<std::string>& fileNames)
{
	fileDirs.clear();
	fileNames.clear();

	intptr_t hFile = 0;  //文件信息    
	_finddata_t fileinfo;//用来存储文件信息的结构体    
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) { //第一次查找     
		do {
			if ((fileinfo.attrib & _A_SUBDIR)) {  //如果查找到的是文件夹  
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {      //进入文件夹查找  
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), fileDirs, fileNames);
				}
			}
			else {   //如果查找到的不是是文件夹   
				fileNames.push_back(p.assign(fileinfo.name));  //将文件路径保存，也可以只保存文件名:    
				fileDirs.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile); //结束查找  
	}
}
//————————————————
//版权声明：本文为CSDN博主「f_pytorch」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https ://blog.csdn.net/CHWB2580/article/details/117470800
//然后我浅改了一下......

void UnicodeOut(const std::wstring &wstr)
{
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), wstr.c_str(), wstr.size(), NULL, NULL);
}


/******************************************************
* 色彩
*
******************************************************/

#define GPU_RGB(r,g,b) ((COLORREF)r + (COLORREF)g * 0x100 + (COLORREF)b * 0x10000)

//RGB->R,G,B
template<typename T> void SplitRgb(COLORREF rgb, T* r, T* g, T* b)
{
	*r = GetRValue(rgb);
	*g = GetGValue(rgb);
	*b = GetBValue(rgb);
}
#ifdef _AMP_H
template<typename T> void SplitRgb(COLORREF rgb, T* r, T* g, T* b) restrict(amp)
{
	*b = rgb / 0x10000;
	rgb -= (int)*b * 0x10000;

	*g = rgb / 0x100;
	rgb -= (int)*g * 0x100;

	*r = rgb;
}
#endif

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

	max = max(r, max(g, b));
	min = min(r, min(g, b));

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
#ifdef _AMP_H
COLORREF HsvToRgb(Func_F h, Func_F s, Func_F v) restrict(amp)
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
	return GPU_RGB(r * 255, g * 255, b * 255);
}
#endif

//渐变色
COLORREF RgbCC(COLORREF start, COLORREF end, Func_F rate)
{
	int r1, g1, b1, r2, g2, b2;
	SplitRgb(start, &r1, &g1, &b1);
	SplitRgb(end, &r2, &g2, &b2);
	return RGB(LnrC(r1, r2, rate), LnrC(g1, g2, rate), LnrC(b1, b2, rate));
}
#ifdef _AMP_H
COLORREF RgbCC(COLORREF start, COLORREF end, Func_F rate) restrict(amp)
{
	int r1, g1, b1, r2, g2, b2;
	SplitRgb(start, &r1, &g1, &b1);
	SplitRgb(end, &r2, &g2, &b2);
	return GPU_RGB(LnrC(r1, r2, rate), LnrC(g1, g2, rate), LnrC(b1, b2, rate));
}
#endif

//HSV渐变色
COLORREF HsvCC(COLORREF start, COLORREF end, Func_F rate)
{
	Func_F h1, s1, v1, h2, s2, v2;
	RgbToHsv(start, &h1, &s1, &v1);
	RgbToHsv(end, &h2, &s2, &v2);
	return HsvToRgb(LnrC(h1, h2, rate), LnrC(s1, s2, rate), LnrC(v1, v2, rate));
}

//预设渐变色
/*presets:渐变预设
* 1		蓝-绿-黄-红
* 2		黑-白
* 3		紫-绿-黄
* 4		蓝-白-红
* 5		白-蓝
* 6		黑紫-黄
*/
COLORREF PresetCC(int preset, Func_F rate)
{
	switch (preset)
	{
	case 1:
		return HsvToRgb(LnrC(240, 0, rate), 100, 100);

	case 2:
		return RgbCC(GPU_RGB(0, 0, 0), GPU_RGB(255, 255, 255), rate);

	case 3:
		if (rate < 0.5)
		{
			return RgbCC(GPU_RGB(70, 0, 90), GPU_RGB(30, 150, 150), rate / 0.5);
		}
		else
		{
			return RgbCC(GPU_RGB(30, 150, 150), GPU_RGB(255, 220, 0), (rate - 0.5) / 0.5);
		}

	case 4:
		if (rate < 0.5)
		{
			return HsvToRgb(
				LnrC(240, 180, rate / 0.5),
				LnrC(80, 0, rate / 0.5),
				LnrC(80, 100, rate / 0.5));
		}
		else
		{
			return HsvToRgb(
				LnrC(60, 0, rate / 0.5 - 1),
				LnrC(0, 80, (rate - 0.5) / 0.5),
				LnrC(100, 80, rate / 0.5 - 1));
		}

	case 5:
		return RgbCC(GPU_RGB(255, 255, 255), GPU_RGB(0, 0, 110), pow(rate, (Func_F)2.4));

	case 6:
	{
		Func_F temp = LnrC(-60, 60, rate);
		if (temp < 0)temp += 360;
		return HsvToRgb(temp, 100, LnrC(0, 100, rate));
	}

	default:
		return GPU_RGB(0, 0, 0);
	}
}
#ifdef _AMP_H
COLORREF PresetCC(int preset, Func_F rate) restrict(amp)
{
	switch (preset)
	{
	case 1:
		return HsvToRgb(LnrC(240, 0, rate), 100, 100);

	case 2:
		return RgbCC(GPU_RGB(0, 0, 0), GPU_RGB(255, 255, 255), rate);

	case 3:
		if (rate < 0.5)
		{
			return RgbCC(GPU_RGB(70, 0, 90), GPU_RGB(30, 150, 150), rate / 0.5);
		}
		else
		{
			return RgbCC(GPU_RGB(30, 150, 150), GPU_RGB(255, 220, 0), (rate - 0.5) / 0.5);
		}

	case 4:
		if (rate < 0.5)
		{
			return HsvToRgb(
				LnrC(240, 180, rate / 0.5),
				LnrC(80, 0, rate / 0.5),
				LnrC(80, 100, rate / 0.5));
		}
		else
		{
			return HsvToRgb(
				LnrC(60, 0, rate / 0.5 - 1),
				LnrC(0, 80, (rate - 0.5) / 0.5),
				LnrC(100, 80, rate / 0.5 - 1));
		}

	case 5:
		return RgbCC(GPU_RGB(255, 255, 255), GPU_RGB(0, 0, 110), pow(rate, (Func_F)2.4));

	case 6:
	{
		Func_F temp = LnrC(-60, 60, rate);
		if (temp < 0)temp += 360;
		return HsvToRgb(temp, 100, LnrC(0, 100, rate));
	}

	default:
		return GPU_RGB(0, 0, 0);
	}
}
#endif