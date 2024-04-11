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

		Angle()
		{
			fac = 0;
		}
		Angle(Deg deg)
		{
			fac = deg.d / 180;
		}
		Angle(Rad rad)
		{
			fac = rad.r / M_PI;
		}


		Math_F get_deg() const
		{
			return fac * 180;
		}
		Math_F get_rad() const
		{
			return fac * M_PI;
		}

		//简化
		void sim()
		{
			while (fac > 2)fac -= 2;
			while (fac < 0)fac += 2;
		}

		//获取两个角的差(不加绝对值)(小于平角)
		static Angle dfc(Angle a, Angle b)
		{
			Angle res;
			res.fac = a.fac - b.fac;
			//分类讨论
			a.sim();
			b.sim();
			if (a.fac > 0 && a.fac < 0.5 && b.fac > 1.5 ||
				a.fac > 1.5 && b.fac > 0 && b.fac < 0.5)
			{
				res.fac = 2 + res.fac;
			}
			return res;
		}

		//运算
		Angle operator=(const Angle& b)
		{
			fac = b.fac;
			return *this;
		}
		//相反方向的角
		Angle operator-() const
		{
			Angle res;
			res.fac = fac + 1;
			return res;
		}

		Angle operator+(const Angle& b) const
		{
			Angle res;
			res.fac = fac + b.fac;
			return res;
		}
		Angle operator-(const Angle& b) const
		{
			Angle res;
			res.fac = fac - b.fac;
			return res;
		}

		Math_F operator*(const Angle& b) const
		{
			return fac * b.fac * M_PI;
		}
		Math_F operator/(const Angle& b) const
		{
			return fac / b.fac;
		}

		Angle operator*(const Math_F f) const
		{
			Angle res;
			res.fac = fac * f;
			return res;
		}
		Angle operator/(const Math_F f) const
		{
			Angle res;
			res.fac = fac / f;
			return res;
		}

		Angle friend operator*(const Math_F f, const Angle& b)
		{
			Angle res;
			res.fac = f * b.fac;
			return res;
		}
		Angle friend operator/(const Math_F f, const Angle& b)
		{
			Angle res;
			res.fac = f / b.fac;
			return res;
		}

		Angle operator+=(const Angle& b)
		{
			*this = *this + b;
			return *this;
		}
		Angle operator-=(const Angle& b)
		{
			*this = *this - b;
			return *this;
		}
		Angle operator*=(const Math_F f)
		{
			*this = *this * f;
			return *this;
		}
		Angle operator/=(const Math_F f)
		{
			*this = *this / f;
			return *this;
		}

		//关系运算
		bool operator==(const Angle& b) const
		{
			return fac == b.fac;
		}
		bool operator!=(const Angle& b) const
		{
			return !(*this == b);
		}
		bool operator>(const Angle& b) const
		{
			return fac > b.fac;
		}
		bool operator<(const Angle& b) const
		{
			return fac < b.fac;
		}
		bool operator>=(const Angle& b) const
		{
			return !(*this < b);
		}
		bool operator<=(const Angle& b) const
		{
			return !(*this > b);
		}
	};
	typedef Angle::Deg Deg;
	typedef Angle::Rad Rad;

	//三角函数支持
	inline Math_F sin(Angle a)
	{
		return ::sin(a.get_rad());
	}
	inline Math_F cos(Angle a)
	{
		return ::cos(a.get_rad());
	}
	inline Math_F tan(Angle a)
	{
		return ::tan(a.get_rad());
	}
	inline Angle arcsin(Math_F f)
	{
		return (Rad)::asin(f);
	}
	inline Angle arccos(Math_F f)
	{
		return (Rad)::acos(f);
	}
	inline Angle arctan(Math_F f)
	{
		return (Rad)::atan(f);
	}


	/*****************************************************************************
	* Rect
	* 矩形范围
	*****************************************************************************/
	class Rect
	{
	public:
		Math_F left, top, right, bottom;

		Rect()
		{
			left = top = right = bottom = 0;
		}
		Rect(Math_F _left, Math_F _top, Math_F _right, Math_F _bottom)
		{
			left = _left;
			top = _top;
			right = _right;
			bottom = _bottom;
		}
		//允许隐式收缩转换
		template<typename T1, typename T2, typename T3, typename T4>
		Rect(T1 _left, T2 _top, T3 _right, T4 _bottom)
		{
			left = _left;
			top = _top;
			right = _right;
			bottom = _bottom;
		}

		//兼容
		Rect(RECT& r)
		{
			left = r.left;
			top = r.top;
			right = r.right;
			bottom = r.bottom;
		}
		operator RECT() const
		{
			return { (LONG)left,(LONG)top,(LONG)right,(LONG)bottom };
		}
	};

	/*****************************************************************************
	* Point
	* 点
	*****************************************************************************/
	class Point
	{
	public:
		Math_F x, y;

		Point()
		{
			x = y = 0;
		}
		Point(Math_F _x, Math_F _y)
		{
			x = _x;
			y = _y;
		}
		//允许隐式收缩转换
		Point(int _x, int _y)
		{
			x = _x;
			y = _y;
		}

		//兼容
		operator POINT() const
		{
			return { (LONG)x,(LONG)y };
		}
	};

	/*****************************************************************************
	* Vector
	* 向量(几何意义)
	*****************************************************************************/
	class Vector
	{
	private:
		void sim()
		{
			if (len < 0)
			{
				theta = -theta;
				len = -len;
			}
			theta.sim();
		}
	public:
		Angle theta;	//方向
		Math_F len;		//长度(模)

		Vector()
		{
			theta = (Deg)0;
			len = 0;
		}
		Vector(Angle _theta, Math_F _len)
		{
			theta = _theta;
			len = _len;
			sim();
		}
		Vector(Math_F x, Math_F y)
		{
			len = sqrt(::pow(x, 2) + ::pow(y, 2));
			if (len != 0)theta = arcsin(y / len);
			else theta = (Deg)0;
			//分类讨论
			if (x < 0)theta = (Angle)(Deg)180 - theta;

			sim();
		}
		Vector(Math_F x0, Math_F y0, Math_F x1, Math_F y1)
		{
			*this = Vector(x1 - x0, y1 - y0);
		}

		//归零
		void clear()
		{
			theta = (Deg)0;
			len = 0;
		}

		Math_F get_x() const
		{
			return len * cos(theta);
		}
		Math_F get_y() const
		{
			return len * sin(theta);
		}
		Point get_p(Math_F x0 = 0, Math_F y0 = 0) const
		{
			return { x0 + get_x(), y0 + get_y() };
		}

		//运算
		Vector operator=(const Vector& b)
		{
			theta = b.theta;
			len = b.len;
			return *this;
		}
		Vector operator-() const
		{
			return Vector(theta + (Deg)180, len);
		}

		Vector operator+(const Vector& b) const
		{
			return Vector(get_x() + b.get_x(), get_y() + b.get_y());
		}
		Vector operator-(const Vector& b) const
		{
			return Vector(get_x() - b.get_x(), get_y() - b.get_y());
		}
		Vector operator*(Math_F f) const
		{
			return Vector(f < 0 ? -theta : theta, len * fabs(f));
		}
		Vector operator/(Math_F f) const
		{
			return Vector(f < 0 ? -theta : theta, len / fabs(f));
		}

		Vector friend operator*(const Math_F f, const Vector& v)
		{
			return v * f;
		}

		Vector operator+=(const Vector& b)
		{
			*this = *this + b;
			return *this;
		}
		Vector operator-=(const Vector& b)
		{
			*this = *this - b;
			return *this;
		}
		Vector operator*=(const Math_F f)
		{
			*this = *this * f;
			return *this;
		}
		Vector operator/=(const Math_F f)
		{
			*this = *this / f;
			return *this;
		}

		//关系运算
		bool operator==(const Vector& b) const
		{
			return theta == b.theta && len == b.len;
		}
		bool operator!=(const Vector& b) const
		{
			return !(*this == b);
		}
		bool operator>(const Vector& b) const
		{
			return len > b.len;
		}
		bool operator<(const Vector& b) const
		{
			return len < b.len;
		}
		bool operator>=(const Vector& b) const
		{
			return !(*this < b);
		}
		bool operator<=(const Vector& b) const
		{
			return !(*this > b);
		}

		//分量
		void split(Angle thetaA, Angle thetaB, Vector* a, Vector* b)
		{
			Angle alpha = theta - thetaA;
			Angle beta = thetaB - theta;
			*a = { thetaA, len * sin(beta) / sin(alpha + beta) };
			*b = { thetaB, len * sin(alpha) / sin(alpha + beta) };
		}
		//垂直分量
		Vector split(Angle thetaV)
		{
			Angle alpha = theta - thetaV;
			Vector res(thetaV, len * cos(alpha));
			//千万确保度数一致,反Sim()
			if (res.theta != thetaV)
			{
				res.theta = -res.theta;
				res.len = -res.len;
			}
			return res;
		}

		//绘制
#ifdef EX_SHOWCONSOLE	//easyx.h中的宏定义
		void draw(Math_F x, Math_F y, Math_F zoom = 1, COLORREF color = RGB(255, 255, 255))
		{
			setlinecolor(color);
			setfillcolor(color);
			setlinestyle(PS_SOLID, 2);

			POINT pt;
			pt = Vector(theta, len * zoom).get_p(x, y);
			line(x, y, pt.x, pt.y);

			setlinestyle(PS_SOLID, 1);
			POINT pts[3];
			pts[1] = pt;
			pts[0] = Vector(theta + (Deg)(180 + 30), min(20, len * zoom / 2)).get_p(pt.x, pt.y);
			pts[2] = Vector(theta - (Deg)(180 + 30), min(20, len * zoom / 2)).get_p(pt.x, pt.y);
			fillpolygon(pts, 3);
		}
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

		Line()
		{
			a = b = c = 0;
		}
		Line(Math_F _a, Math_F _b, Math_F _c)
		{
			a = _a;
			b = _b;
			c = _c;
		}
		Line(Point pt1, Point pt2)
		{
			//a(x1-x2)+b(y1-y2)=0
			if (pt1.x == pt2.x)
			{
				b = 0;
			}
			else if (pt1.y == pt2.y)
			{
				a = 0;
			}
			else
			{
				//a(x1-x2)=-b(y1-y2)
				//b=-a(x1-x2)/(y1-y2)
				a = 1;
				b = -a * (pt1.x - pt2.x) / (pt1.y - pt2.y);
			}
			c = -a * pt1.x - b * pt1.y;
		}

		//是否平行于
		bool is_prl(const Line& l)const
		{
			//不合法情况
			if (a == 0 && b == 0 || l.a == 0 && l.b == 0)return false;

			//平行于坐标轴的情况
			if (a == 0 && l.a == 0)return true;
			if (b == 0 && l.b == 0)return true;

			if (a / b == l.a / l.b)return true;
			return false;
		}
		//是否垂直于
		bool is_vrt(const Line& l)const
		{
			//不合法情况
			if (a == 0 && b == 0 || l.a == 0 && l.b == 0)return false;

			//平行于坐标轴的情况
			if (a == 0 && l.b == 0)return true;
			if (b == 0 && l.a == 0)return true;

			if (a / b * l.a / l.b == -1)return true;
			return false;
		}

		//关系运算
		bool operator==(const Line& l) const
		{
			//不合法情况
			if (a == 0 && b == 0 || l.a == 0 && l.b == 0)return false;

			//平行于坐标轴的情况
			if (a == 0 && l.a != 0)return false;
			if (b == 0 && l.b != 0)return false;
			//平行于x轴, y=-c/b
			if (a == 0 && c / b == l.c / l.b)return true;
			//平行于y轴, x=-c/a
			if (b == 0 && c / a == l.c / l.a)return true;

			//y=-a/b*x-c/b
			if (a / b == l.a / l.b &&
				c / b == l.c / l.b)
			{
				return true;
			}
			return false;
		}
		bool operator!=(const Line& l) const
		{
			return !(*this == l);
		}
	};
}