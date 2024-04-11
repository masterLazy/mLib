#pragma once
/*****************************************************************************
* numEx.h
* 高精度数字
*
*****************************************************************************/

namespace mlib
{
	/*****************************************************************************
	* IntEx
	* 高精度整数
	*****************************************************************************/
	class IntEx
	{
	private:
		std::vector<char> arr;
		bool neg = false; //是否为负
	public:
		//构造
		IntEx() {}
		IntEx(unsigned int x);
		IntEx(int x);
		IntEx(const std::string& x);

		//数字长度
		size_t size() const;

		//转为 string
		std::string get_str(bool positive_sign = false) const;

		//比较大小
		bool operator==(const IntEx& b) const;
		bool operator!=(const IntEx& b) const;
		bool operator>(const IntEx& b) const;
		bool operator>=(const IntEx& b) const;
		bool operator<(const IntEx& b) const;
		bool operator<=(const IntEx& b) const;

		//运算
		IntEx operator-() const;

		IntEx operator+(const IntEx& b) const;
		IntEx operator-(const IntEx& b) const;

		//控制台I/O
#ifdef _IOSTREAM_
		friend std::istream& operator>>(std::istream& is, IntEx& i);
		friend std::ostream& operator<<(std::ostream& os, const IntEx& i);
#endif
	};
}