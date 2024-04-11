#pragma once
/*****************************************************************************
* algebra
* ����
*
*****************************************************************************/

namespace mlib
{
	/*****************************************************************************
	* Complex
	* ����
	*****************************************************************************/
	class Complex
	{
	public:
		Math_F r;	//ʵ��
		Math_F i;	//�鲿

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
		//ģ
		Math_F mag()
		{
			return sqrt(::pow(r, 2) + ::pow(i, 2));
		}

		//����
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

		//��ϵ����
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