#pragma once
/*****************************************************************************
* ele_algebra
* ���ȴ���
*
*****************************************************************************/

namespace mlib
{
	/*****************************************************************************
	* Complex
	* ����
	*
	*****************************************************************************/
	class Complex
	{
	public:
		Math_F r;	//ʵ��
		Math_F i;	//�鲿

		Complex() _RST
		{
			r = i = 0;
		}
		Complex(Math_F _re, Math_F _im) _RST
		{
			r = _re;
			i = _im;
		}

		void set(Math_F _re, Math_F _im) _RST
		{
			r = _re;
			i = _im;
		}
		//ģ
		Math_F mag()
		{
			return sqrt(pow(r, 2) + pow(i, 2));
		}

		//����
		Complex operator=(const Complex& b) _RST
		{
			r = b.r;
			i = b.i;
			return *this;
		}
		Complex operator-() const _RST
		{
			return Complex(-r, -i);
		}

		Complex operator+(const Complex& b) const _RST
		{
			return Complex(r + b.r, i + b.i);
		}
		Complex operator-(const Complex& b) const _RST
		{
			return Complex(r - b.r, i - b.i);
		}
		Complex operator*(const Complex& b) const _RST
		{
			return Complex(
				r * b.r - i * b.i,
				b.r * i + r * b.i
			);
		}
		Complex operator/(const Complex& b) const _RST
		{
			return Complex(
				(r * b.r + i * b.i) / pow(i, 2) + pow(b.i, 2),
				(b.r * i - r * b.i) / pow(i, 2) + pow(b.i, 2)
			);
		}

		Complex operator+=(const Complex& b) _RST
		{
			*this = *this + b;
			return *this;
		}
		Complex operator-=(const Complex& b) _RST
		{
			*this = *this - b;
			return *this;
		}
		Complex operator*=(const Complex& b) _RST
		{
			*this = *this * b;
			return *this;
		}
		Complex operator/=(const Complex& b) _RST
		{
			*this = *this / b;
			return *this;
		}

		//��ϵ����
		bool operator==(const Complex& b) _RST
		{
			return r == b.r && i == b.i;
		}
		bool operator!=(const Complex& b) _RST
		{
			return !(*this == b);
		}
	};
}