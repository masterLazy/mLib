#pragma once
/*****************************************************************************
* geometry.h
* ����
*
*****************************************************************************/

namespace mlib
{
	/*****************************************************************************
	* Angle
	* ��
	*****************************************************************************/
	class Angle
	{
		Math_F fac;	//����,*this����ʾ�ĽǶ�=fac*M_PI rad
	public:

		//����������
		class Deg
		{
		public:
			Math_F d;
			Deg(Math_F _d)
			{
				d = _d;
			}
		};
		class Rad
		{
		public:
			Math_F r;
			Rad(Math_F _r)
			{
				r = _r;
			}
		};

		Angle();
		Angle(Deg deg);
		Angle(Rad rad);


		Math_F get_deg() const;
		Math_F get_rad() const;

		//��
		void sim();

		//��ȡ�����ǵĲ�(���Ӿ���ֵ)(С��ƽ��)
		static Angle dfc(Angle a, Angle b);

		//����
		Angle operator=(const Angle& b);
		//�෴����Ľ�
		Angle operator-() const;

		Angle operator+(const Angle& b) const;
		Angle operator-(const Angle& b) const;

		Math_F operator*(const Angle& b) const;
		Math_F operator/(const Angle& b) const;

		Angle operator*(const Math_F f) const;
		Angle operator/(const Math_F f) const;

		friend Angle operator*(const Math_F f, const Angle& b);
		friend Angle operator/(const Math_F f, const Angle& b);

		Angle operator+=(const Angle& b);
		Angle operator-=(const Angle& b);
		Angle operator*=(const Math_F f);
		Angle operator/=(const Math_F f);

		//��ϵ����
		bool operator==(const Angle& b) const;
		bool operator!=(const Angle& b) const;
		bool operator>(const Angle& b) const;
		bool operator<(const Angle& b) const;
		bool operator>=(const Angle& b) const;
		bool operator<=(const Angle& b) const;
	};
	typedef Angle::Deg Deg;
	typedef Angle::Rad Rad;

	//���Ǻ���֧��
	inline Math_F sin(Angle a);
	inline Math_F cos(Angle a);
	inline Math_F tan(Angle a);
	inline Angle arcsin(Math_F f);
	inline Angle arccos(Math_F f);
	inline Angle arctan(Math_F f);


	/*****************************************************************************
	* Rect
	* ���η�Χ
	*****************************************************************************/
	class Rect
	{
	public:
		Math_F left, top, right, bottom;

		Rect();
		Rect(Math_F _left, Math_F _top, Math_F _right, Math_F _bottom);
		//������ʽ����ת��
		template<typename T1, typename T2, typename T3, typename T4>
		Rect(T1 _left, T2 _top, T3 _right, T4 _bottom);

		//����
		Rect(RECT& r);
		operator RECT() const;
	};

	/*****************************************************************************
	* Point
	* ��
	*****************************************************************************/
	class Point
	{
	public:
		Math_F x, y;

		Point();
		Point(Math_F _x, Math_F _y);
		//������ʽ����ת��
		Point(int _x, int _y);

		//����
		operator POINT() const;
	};

	/*****************************************************************************
	* Vector
	* ����(��������)
	*****************************************************************************/
	class Vector
	{
	private:
		void sim();
	public:
		Angle theta;	//����
		Math_F len;		//����(ģ)

		Vector();
		Vector(Angle _theta, Math_F _len);
		Vector(Math_F x, Math_F y);
		Vector(Math_F x0, Math_F y0, Math_F x1, Math_F y1);

		//����
		void clear();

		Math_F get_x() const;
		Math_F get_y() const;
		Point get_p(Math_F x0 = 0, Math_F y0 = 0) const;

		//����
		Vector operator=(const Vector& b);
		Vector operator-() const;

		Vector operator+(const Vector& b) const;
		Vector operator-(const Vector& b) const;
		Vector operator*(Math_F f) const;
		Vector operator/(Math_F f) const;

		Vector friend operator*(const Math_F f, const Vector& v);

		Vector operator+=(const Vector& b);
		Vector operator-=(const Vector& b);
		Vector operator*=(const Math_F f);
		Vector operator/=(const Math_F f);

		//��ϵ����
		bool operator==(const Vector& b) const;
		bool operator!=(const Vector& b) const;
		bool operator>(const Vector& b) const;
		bool operator<(const Vector& b) const;
		bool operator>=(const Vector& b) const;
		bool operator<=(const Vector& b) const;

		//����
		void split(Angle thetaA, Angle thetaB, Vector* a, Vector* b);
		//��ֱ����
		Vector split(Angle thetaV);

		//����
#ifdef EX_SHOWCONSOLE	//easyx.h�еĺ궨��
		void draw(Math_F x, Math_F y, Math_F zoom = 1, COLORREF color = RGB(255, 255, 255));
#endif
	};
	typedef Vector Vec;

	/*****************************************************************************
	* Line
	* ֱ��
	*****************************************************************************/
	class Line
	{
	public:
		//ֱ���� ax+by+c=0 ��ʾ
		Math_F a, b, c;

		Line();
		Line(Math_F _a, Math_F _b, Math_F _c);
		Line(Point pt1, Point pt2);

		//�Ƿ�ƽ����
		bool is_prl(const Line& l)const;
		//�Ƿ�ֱ��
		bool is_vrt(const Line& l)const;

		//��ϵ����
		bool operator==(const Line& l) const;
		bool operator!=(const Line& l) const;
	};
}