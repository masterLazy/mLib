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

		Complex();
		Complex(Math_F re);
		Complex(Math_F re, Math_F im);

		void set(Math_F re, Math_F im);
		//ģ
		Math_F mag();

		//����
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

		//��ϵ����
		bool operator==(const Complex& b);
		bool operator!=(const Complex& b);
	};
}