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

		Complex();
		Complex(Math_F re);
		Complex(Math_F re, Math_F im);

		void set(Math_F re, Math_F im);
		//模
		Math_F mag();

		//运算
		Complex operator=(const Complex& b);
		Complex operator-() const;

		Complex operator+(const Complex& b) const;
		Complex operator-(const Complex& b) const;
		Complex operator*(const Complex& b) const;
		Complex operator/(const Complex& b) const;

		Complex operator+=(const Complex& b);
		Complex operator-=(const Complex& b);
		Complex operator*=(const Complex& b);
		Complex operator/=(const Complex& b);

		//关系运算
		bool operator==(const Complex& b);
		bool operator!=(const Complex& b);
	};
}