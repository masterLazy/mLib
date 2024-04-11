#pragma once
/*****************************************************************************
* algebra
* 代数
*
*****************************************************************************/

namespace mlib
{
	/*****************************************************************************
	* Complex
	* 复数
	*****************************************************************************/
	class Complex
	{
	public:
		Math_F r;	//实部
		Math_F i;	//虚部

		Complex()
		{
			r = i = 0;
		}
		Complex(Math_F re)
		{
			r = re;
			i = 0;
		}
		Complex(Math_F re, Math_F im)
		{
			r = re;
			i = im;
		}

		void set(Math_F re, Math_F im)
		{
			r = re;
			i = im;
		}
		//模
		Math_F mag()
		{
			return sqrt(::pow(r, 2) + ::pow(i, 2));
		}

		//运算
		Complex operator=(const Complex& b)
		{
			r = b.r;
			i = b.i;
			return *this;
		}
		Complex operator-() const
		{
			return Complex(-r, -i);
		}

		Complex operator+(const Complex& b) const
		{
			return Complex(r + b.r, i + b.i);
		}
		Complex operator-(const Complex& b) const
		{
			return Complex(r - b.r, i - b.i);
		}
		Complex operator*(const Complex& b) const
		{
			return Complex(
				r * b.r - i * b.i,
				b.r * i + r * b.i
			);
		}
		Complex operator/(const Complex& b) const
		{
			return Complex(
				(r * b.r + i * b.i) / ::pow(i, 2) + ::pow(b.i, 2),
				(b.r * i - r * b.i) / ::pow(i, 2) + ::pow(b.i, 2)
			);
		}

		Complex operator+=(const Complex& b)
		{
			*this = *this + b;
			return *this;
		}
		Complex operator-=(const Complex& b)
		{
			*this = *this - b;
			return *this;
		}
		Complex operator*=(const Complex& b)
		{
			*this = *this * b;
			return *this;
		}
		Complex operator/=(const Complex& b)
		{
			*this = *this / b;
			return *this;
		}

		//关系运算
		bool operator==(const Complex& b)
		{
			return r == b.r && i == b.i;
		}
		bool operator!=(const Complex& b)
		{
			return !(*this == b);
		}
	};
}