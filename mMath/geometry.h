#pragma once
/*****************************************************************************
* geometry.h
* 几何
*
*****************************************************************************/

namespace mlib
{
	/*****************************************************************************
	* Angle
	* 角
	*****************************************************************************/
	class Angle
	{
		Math_F fac;	//因数,*this所表示的角度=fac*M_PI rad
	public:

		//两个辅助类
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

		//简化
		void sim();

		//获取两个角的差(不加绝对值)(小于平角)
		static Angle dfc(Angle a, Angle b);

		//运算
		Angle operator=(const Angle& b);
		//相反方向的角
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

		//关系运算
		bool operator==(const Angle& b) const;
		bool operator!=(const Angle& b) const;
		bool operator>(const Angle& b) const;
		bool operator<(const Angle& b) const;
		bool operator>=(const Angle& b) const;
		bool operator<=(const Angle& b) const;
	};
	typedef Angle::Deg Deg;
	typedef Angle::Rad Rad;

	//三角函数支持
	inline Math_F sin(Angle a);
	inline Math_F cos(Angle a);
	inline Math_F tan(Angle a);
	inline Angle arcsin(Math_F f);
	inline Angle arccos(Math_F f);
	inline Angle arctan(Math_F f);


	/*****************************************************************************
	* Rect
	* 矩形范围
	*****************************************************************************/
	class Rect
	{
	public:
		Math_F left, top, right, bottom;

		Rect();
		Rect(Math_F _left, Math_F _top, Math_F _right, Math_F _bottom);
		//允许隐式收缩转换
		template<typename T1, typename T2, typename T3, typename T4>
		Rect(T1 _left, T2 _top, T3 _right, T4 _bottom);

		//兼容
		Rect(RECT& r);
		operator RECT() const;
	};

	/*****************************************************************************
	* Point
	* 点
	*****************************************************************************/
	class Point
	{
	public:
		Math_F x, y;

		Point();
		Point(Math_F _x, Math_F _y);
		//允许隐式收缩转换
		Point(int _x, int _y);

		//兼容
		operator POINT() const;
	};

	/*****************************************************************************
	* Vector
	* 向量(几何意义)
	*****************************************************************************/
	class Vector
	{
	private:
		void sim();
	public:
		Angle theta;	//方向
		Math_F len;		//长度(模)

		Vector();
		Vector(Angle _theta, Math_F _len);
		Vector(Math_F x, Math_F y);
		Vector(Math_F x0, Math_F y0, Math_F x1, Math_F y1);

		//归零
		void clear();

		Math_F get_x() const;
		Math_F get_y() const;
		Point get_p(Math_F x0 = 0, Math_F y0 = 0) const;

		//运算
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

		//关系运算
		bool operator==(const Vector& b) const;
		bool operator!=(const Vector& b) const;
		bool operator>(const Vector& b) const;
		bool operator<(const Vector& b) const;
		bool operator>=(const Vector& b) const;
		bool operator<=(const Vector& b) const;

		//分量
		void split(Angle thetaA, Angle thetaB, Vector* a, Vector* b);
		//垂直分量
		Vector split(Angle thetaV);

		//绘制
#ifdef EX_SHOWCONSOLE	//easyx.h中的宏定义
		void draw(Math_F x, Math_F y, Math_F zoom = 1, COLORREF color = RGB(255, 255, 255));
#endif
	};
	typedef Vector Vec;

	/*****************************************************************************
	* Line
	* 直线
	*****************************************************************************/
	class Line
	{
	public:
		//直线用 ax+by+c=0 表示
		Math_F a, b, c;

		Line();
		Line(Math_F _a, Math_F _b, Math_F _c);
		Line(Point pt1, Point pt2);

		//是否平行于
		bool is_prl(const Line& l)const;
		//是否垂直于
		bool is_vrt(const Line& l)const;

		//关系运算
		bool operator==(const Line& l) const;
		bool operator!=(const Line& l) const;
	};
}