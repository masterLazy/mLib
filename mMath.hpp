#pragma once
/*****************************************************************************
* mMath.hpp
* 数学
*
*****************************************************************************/

inline Math_F mlib::Map(Math_F x, Math_F x_min, Math_F x_max, Math_F y_min, Math_F y_max)
{
	//归一化
	Math_F one = (x - x_min) / (x_max - x_min);
	//映射
	Math_F m_map = one * (y_max - y_min) + y_min;
	//确保范围
	//if (m_map < y_min)return y_min;
	//else if (m_map > y_max)return y_max;
	return m_map;
}

/*****************************************************************************
* Particle
* 质点
*****************************************************************************/

mlib::Particle::Particle()
{
	m = 0;
	x = y = 0;
	x0 = x;
	y0 = y;
}
mlib::Particle::Particle(Math_F _m, Math_F _x, Math_F _y)
{
	m = _m;
	x = _x;
	y = _y;
	x0 = x;
	y0 = y;
}
mlib::Particle::Particle(Math_F _m, Math_F _x, Math_F _y, Vector v0)
{
	m = _m;
	x = _x;
	y = _y;
	v = v0;
	x0 = x;
	y0 = y;
}

void mlib::Particle::set(Math_F _x, Math_F _y)
{
	x = _x;
	y = _y;
	x0 = x;
	y0 = y;
}
void mlib::Particle::set(Math_F _m, Math_F _x, Math_F _y)
{
	m = _m;
	x = _x;
	y = _y;
	x0 = x;
	y0 = y;
}
void mlib::Particle::set(Math_F _m, Math_F _x, Math_F _y, Vector v0)
{
	m = _m;
	x = _x;
	y = _y;
	v = v0;
	x0 = x;
	y0 = y;
	v = v0;
}

void mlib::Particle::set_x(Math_F _x)
{
	x = _x;
	x0 = x;
}
void mlib::Particle::set_y(Math_F _y)
{
	y = _y;
	y0 = y;
}

Math_F mlib::Particle::get_x() const
{
	return x;
}
Math_F mlib::Particle::get_y() const
{
	return y;
}

mlib::Particle mlib::Particle::operator=(const Particle& par)
{
	x0 = par.x0;
	y0 = par.y0;
	x = par.x;
	y = par.y;
	F_all = par.F_all;
	m = par.m;
	v = par.v;
	return *this;
}

void mlib::Particle::clr_force()
{
	F_all.clear();
}
void mlib::Particle::add_force(Vec F_new)
{
	F_all += F_new;
}
mlib::Vec mlib::Particle::get_force()
{
	return F_all;
}
void mlib::Particle::wrk_force(Math_F tick)
{
	//存下当前质点的坐标
	Math_F x_temp, y_temp;
	x_temp = x;
	y_temp = y;

	//计算加速度
	Vector a;
	a = F_all / m;

	//Verlet积分法 计算位移
	x = 2 * x - x0 + ::pow(tick, 2) * a.split((Deg)0).len;
	y = 2 * y - y0 + ::pow(tick, 2) * a.split((Deg)90).len;

	//更新"上一帧的坐标"
	x0 = x_temp;
	y0 = y_temp;

	//计算速度
	v = { x0, y0, x, y };

	//清空所受力
	F_all.clear();
}

/*****************************************************************************
* Angle
* 角
*****************************************************************************/

mlib::Angle::Angle()
{
	fac = 0;
}
mlib::Angle::Angle(Deg deg)
{
	fac = deg.d / 180;
}
mlib::Angle::Angle(Rad rad)
{
	fac = rad.r / M_PI;
}


Math_F mlib::Angle::get_deg() const
{
	return fac * 180;
}
Math_F mlib::Angle::get_rad() const
{
	return fac * M_PI;
}

void mlib::Angle::sim()
{
	while (fac > 2)fac -= 2;
	while (fac < 0)fac += 2;
}

mlib::Angle mlib::Angle::dfc(mlib::Angle a, mlib::Angle b)
{
	mlib::Angle res;
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

mlib::Angle mlib::Angle::operator=(const mlib::Angle& b)
{
	fac = b.fac;
	return *this;
}
mlib::Angle mlib::Angle::operator-() const
{
	mlib::Angle res;
	res.fac = fac + 1;
	return res;
}

mlib::Angle mlib::Angle::operator+(const mlib::Angle& b) const
{
	mlib::Angle res;
	res.fac = fac + b.fac;
	return res;
}
mlib::Angle mlib::Angle::operator-(const mlib::Angle& b) const
{
	mlib::Angle res;
	res.fac = fac - b.fac;
	return res;
}

Math_F mlib::Angle::operator*(const mlib::Angle& b) const
{
	return fac * b.fac * M_PI;
}
Math_F mlib::Angle::operator/(const mlib::Angle& b) const
{
	return fac / b.fac;
}

mlib::Angle mlib::Angle::operator*(const Math_F f) const
{
	mlib::Angle res;
	res.fac = fac * f;
	return res;
}
mlib::Angle mlib::Angle::operator/(const Math_F f) const
{
	mlib::Angle res;
	res.fac = fac / f;
	return res;
}

mlib::Angle mlib::operator*(const Math_F f, const mlib::Angle& b)
{
	mlib::Angle res;
	res.fac = f * b.fac;
	return res;
}
mlib::Angle mlib::operator/(const Math_F f, const mlib::Angle& b)
{
	mlib::Angle res;
	res.fac = f / b.fac;
	return res;
}

mlib::Angle mlib::Angle::operator+=(const mlib::Angle& b)
{
	*this = *this + b;
	return *this;
}
mlib::Angle mlib::Angle::operator-=(const mlib::Angle& b)
{
	*this = *this - b;
	return *this;
}
mlib::Angle mlib::Angle::operator*=(const Math_F f)
{
	*this = *this * f;
	return *this;
}
mlib::Angle mlib::Angle::operator/=(const Math_F f)
{
	*this = *this / f;
	return *this;
}

bool mlib::Angle::operator==(const mlib::Angle& b) const
{
	return fac == b.fac;
}
bool mlib::Angle::operator!=(const mlib::Angle& b) const
{
	return !(*this == b);
}
bool mlib::Angle::operator>(const mlib::Angle& b) const
{
	return fac > b.fac;
}
bool mlib::Angle::operator<(const mlib::Angle& b) const
{
	return fac < b.fac;
}
bool mlib::Angle::operator>=(const mlib::Angle& b) const
{
	return !(*this < b);
}
bool mlib::Angle::operator<=(const mlib::Angle& b) const
{
	return !(*this > b);
}

inline Math_F mlib::sin(mlib::Angle a)
{
	return ::sin(a.get_rad());
}
inline Math_F mlib::cos(mlib::Angle a)
{
	return ::cos(a.get_rad());
}
inline Math_F mlib::tan(mlib::Angle a)
{
	return ::tan(a.get_rad());
}
inline mlib::Angle mlib::arcsin(Math_F f)
{
	return (Rad)::asin(f);
}
inline mlib::Angle mlib::arccos(Math_F f)
{
	return (Rad)::acos(f);
}
inline mlib::Angle mlib::arctan(Math_F f)
{
	return (Rad)::atan(f);
}

/*****************************************************************************
* Rect
* 矩形范围
*****************************************************************************/

mlib::Rect::Rect()
{
	left = top = right = bottom = 0;
}
mlib::Rect::Rect(Math_F _left, Math_F _top, Math_F _right, Math_F _bottom)
{
	left = _left;
	top = _top;
	right = _right;
	bottom = _bottom;
}

template<typename T1, typename T2, typename T3, typename T4>
mlib::Rect::Rect(T1 _left, T2 _top, T3 _right, T4 _bottom)
{
	left = _left;
	top = _top;
	right = _right;
	bottom = _bottom;
}

mlib::Rect::Rect(RECT& r)
{
	left = r.left;
	top = r.top;
	right = r.right;
	bottom = r.bottom;
}
mlib::Rect::operator RECT() const
{
	return { (LONG)left,(LONG)top,(LONG)right,(LONG)bottom };
}

/*****************************************************************************
* Point
* 点
*****************************************************************************/

mlib::Point::Point()
{
	x = y = 0;
}
mlib::Point::Point(Math_F _x, Math_F _y)
{
	x = _x;
	y = _y;
}
mlib::Point::Point(int _x, int _y)
{
	x = _x;
	y = _y;
}

mlib::Point::operator POINT() const
{
	return { (LONG)x,(LONG)y };
}

/*****************************************************************************
* Vector
* 向量(几何意义)
*****************************************************************************/

void mlib::Vector::sim()
{
	if (len < 0)
	{
		theta = -theta;
		len = -len;
	}
	theta.sim();
}

mlib::Vector::Vector()
{
	theta = (Deg)0;
	len = 0;
}
mlib::Vector::Vector(mlib::Angle _theta, Math_F _len)
{
	theta = _theta;
	len = _len;
	sim();
}
mlib::Vector::Vector(Math_F x, Math_F y)
{
	len = sqrt(::pow(x, 2) + ::pow(y, 2));
	if (len != 0)theta = arcsin(y / len);
	else theta = (Deg)0;
	//分类讨论
	if (x < 0)theta = (Angle)(Deg)180 - theta;

	sim();
}
mlib::Vector::Vector(Math_F x0, Math_F y0, Math_F x1, Math_F y1)
{
	*this = Vector(x1 - x0, y1 - y0);
}

void mlib::Vector::clear()
{
	theta = (Deg)0;
	len = 0;
}

Math_F mlib::Vector::get_x() const
{
	return len * cos(theta);
}
Math_F mlib::Vector::get_y() const
{
	return len * sin(theta);
}
mlib::Point mlib::Vector::get_p(Math_F x0, Math_F y0) const
{
	return { x0 + get_x(), y0 + get_y() };
}

mlib::Vector mlib::Vector::operator=(const mlib::Vector& b)
{
	theta = b.theta;
	len = b.len;
	return *this;
}
mlib::Vector mlib::Vector::operator-() const
{
	return Vector(theta + (Deg)180, len);
}

mlib::Vector mlib::Vector::operator+(const mlib::Vector& b) const
{
	return Vector(get_x() + b.get_x(), get_y() + b.get_y());
}
mlib::Vector mlib::Vector::operator-(const mlib::Vector& b) const
{
	return Vector(get_x() - b.get_x(), get_y() - b.get_y());
}
mlib::Vector mlib::Vector::operator*(Math_F f) const
{
	return Vector(f < 0 ? -theta : theta, len * fabs(f));
}
mlib::Vector mlib::Vector::operator/(Math_F f) const
{
	return Vector(f < 0 ? -theta : theta, len / fabs(f));
}

mlib::Vector mlib::operator*(const Math_F f, const mlib::Vector& v)
{
	return v * f;
}

mlib::Vector mlib::Vector::operator+=(const mlib::Vector& b)
{
	*this = *this + b;
	return *this;
}
mlib::Vector mlib::Vector::operator-=(const mlib::Vector& b)
{
	*this = *this - b;
	return *this;
}
mlib::Vector mlib::Vector::operator*=(const Math_F f)
{
	*this = *this * f;
	return *this;
}
mlib::Vector mlib::Vector::operator/=(const Math_F f)
{
	*this = *this / f;
	return *this;
}

bool mlib::Vector::operator==(const mlib::Vector& b) const
{
	return theta == b.theta && len == b.len;
}
bool mlib::Vector::operator!=(const mlib::Vector& b) const
{
	return !(*this == b);
}
bool mlib::Vector::operator>(const mlib::Vector& b) const
{
	return len > b.len;
}
bool mlib::Vector::operator<(const mlib::Vector& b) const
{
	return len < b.len;
}
bool mlib::Vector::operator>=(const mlib::Vector& b) const
{
	return !(*this < b);
}
bool mlib::Vector::operator<=(const mlib::Vector& b) const
{
	return !(*this > b);
}

void mlib::Vector::split(mlib::Angle thetaA, mlib::Angle thetaB,
	mlib::Vector* a, mlib::Vector* b)
{
	Angle alpha = theta - thetaA;
	Angle beta = thetaB - theta;
	*a = { thetaA, len * sin(beta) / sin(alpha + beta) };
	*b = { thetaB, len * sin(alpha) / sin(alpha + beta) };
}
mlib::Vector mlib::Vector::split(mlib::Angle thetaV)
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
void mlib::Vector::draw(Math_F x, Math_F y, Math_F zoom = 1, COLORREF color = RGB(255, 255, 255))
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

/*****************************************************************************
* Line
* 直线
*****************************************************************************/

mlib::Line::Line()
{
	a = b = c = 0;
}
mlib::Line::Line(Math_F _a, Math_F _b, Math_F _c)
{
	a = _a;
	b = _b;
	c = _c;
}
mlib::Line::Line(mlib::Point pt1, mlib::Point pt2)
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

bool mlib::Line::is_prl(const mlib::Line& l)const
{
	//不合法情况
	if (a == 0 && b == 0 || l.a == 0 && l.b == 0)return false;

	//平行于坐标轴的情况
	if (a == 0 && l.a == 0)return true;
	if (b == 0 && l.b == 0)return true;

	if (a / b == l.a / l.b)return true;
	return false;
}
bool mlib::Line::is_vrt(const mlib::Line& l)const
{
	//不合法情况
	if (a == 0 && b == 0 || l.a == 0 && l.b == 0)return false;

	//平行于坐标轴的情况
	if (a == 0 && l.b == 0)return true;
	if (b == 0 && l.a == 0)return true;

	if (a / b * l.a / l.b == -1)return true;
	return false;
}

bool mlib::Line::operator==(const mlib::Line& l) const
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
bool mlib::Line::operator!=(const mlib::Line& l) const
{
	return !(*this == l);
}

/*****************************************************************************
* Complex
* 复数
*****************************************************************************/

mlib::Complex::Complex()
{
	r = i = 0;
}
mlib::Complex::Complex(Math_F re)
{
	r = re;
	i = 0;
}
mlib::Complex::Complex(Math_F re, Math_F im)
{
	r = re;
	i = im;
}

void mlib::Complex::set(Math_F re, Math_F im)
{
	r = re;
	i = im;
}
Math_F mlib::Complex::mag()
{
	return sqrt(::pow(r, 2) + ::pow(i, 2));
}

mlib::Complex mlib::Complex::operator=(const Complex& b)
{
	r = b.r;
	i = b.i;
	return *this;
}
mlib::Complex mlib::Complex::operator-() const
{
	return Complex(-r, -i);
}

mlib::Complex mlib::Complex::operator+(const Complex& b) const
{
	return Complex(r + b.r, i + b.i);
}
mlib::Complex mlib::Complex::operator-(const Complex& b) const
{
	return Complex(r - b.r, i - b.i);
}
mlib::Complex mlib::Complex::operator*(const Complex& b) const
{
	return Complex(
		r * b.r - i * b.i,
		b.r * i + r * b.i
	);
}
mlib::Complex mlib::Complex::operator/(const Complex& b) const
{
	return Complex(
		(r * b.r + i * b.i) / ::pow(i, 2) + ::pow(b.i, 2),
		(b.r * i - r * b.i) / ::pow(i, 2) + ::pow(b.i, 2)
	);
}

mlib::Complex mlib::Complex::operator+=(const Complex& b)
{
	*this = *this + b;
	return *this;
}
mlib::Complex mlib::Complex::operator-=(const Complex& b)
{
	*this = *this - b;
	return *this;
}
mlib::Complex mlib::Complex::operator*=(const Complex& b)
{
	*this = *this * b;
	return *this;
}
mlib::Complex mlib::Complex::operator/=(const Complex& b)
{
	*this = *this / b;
	return *this;
}

bool mlib::Complex::operator==(const Complex& b)
{
	return r == b.r && i == b.i;
}
bool mlib::Complex::operator!=(const Complex& b)
{
	return !(*this == b);
}

/*****************************************************************************
* Value
* 求值链(主要用于自动求导)
*****************************************************************************/

Math_F mlib::Value::get_dvt(const Value& x) const
{
	//自身 
	if (this == &x)return 1;
	//非自身的常量
	else if (opt == Operation::null)return 0;

	Math_F av = a->get();
	Math_F bv = b->get();
	Math_F da = a->get_dvt(x);
	Math_F db = b->get_dvt(x);

	switch (opt)
	{
	case Operation::add:return da + db;
	case Operation::sub:return da - db;
		//[f(x)g(x)]'=f'(x)g(x)+f(x)g'(x)
	case Operation::mlt:return da * bv + av * db;
		//[f(x)/g(x)]'=[f'(x)g(x)-f(x)g'(x)]/[g(x)]^2
	case Operation::div:return (da * bv - av * db) / (bv * bv);
	case Operation::pow:
		if (av == 0)
		{
			return 0;
		}
		else
		{
			//[f(x)^g(x)]'=f(x)^g(x)*{g'(x)*ln[f(x)]+g(x)/f(x)*f'(x)}
			return ::pow(a->get(), bv) * (db * ::log(abs(av)) + bv / av * da);
		}
	}
}

mlib::Value::Value(Math_F x)
{
	opt = Operation::null;
	rv = x;
	a = b = nullptr;
}

Math_F mlib::Value::get() const
{
	switch (opt)
	{
	case Operation::null:return rv;
	case Operation::add:return a->get() + b->get();
	case Operation::sub:return a->get() - b->get();
	case Operation::mlt:return a->get() * b->get();
	case Operation::div:return a->get() / b->get();
	case Operation::pow:return ::pow(a->get(), b->get());
	case Operation::log:return ::log(a->get()) / ::log(b->get());
	case Operation::dvt:return a->get_dvt(*b);
	}
}
Math_F mlib::Value::operator=(Math_F b)
{
	set(b);
	return b;
}

bool mlib::Value::operator==(const mlib::Value& b) const
{
	return get() == b.get();
}
bool mlib::Value::operator!=(const mlib::Value& b) const
{
	return get() != b.get();
}
bool mlib::Value::operator<(const mlib::Value& b) const
{
	return get() < b.get();
}
bool mlib::Value::operator>(const mlib::Value& b) const
{
	return get() > b.get();
}
bool mlib::Value::operator<=(const mlib::Value& b) const
{
	return get() <= b.get();
}
bool mlib::Value::operator>=(const mlib::Value& b) const
{
	return get() >= b.get();
}

/*建立运算关系*/

mlib::Value mlib::Value::operator+(const mlib::Value& b) const
{
	Value res;
	res.a = this;
	res.b = &b;
	res.opt = Operation::add;
	return res;
}
mlib::Value mlib::Value::operator-(const mlib::Value& b) const
{
	Value res;
	res.a = this;
	res.b = &b;
	res.opt = Operation::sub;
	return res;
}
mlib::Value mlib::Value::operator*(const mlib::Value& b) const
{
	Value res;
	res.a = this;
	res.b = &b;
	res.opt = Operation::mlt;
	return res;
}
mlib::Value mlib::Value::operator/(const mlib::Value& b) const
{
	Value res;
	res.a = this;
	res.b = &b;
	res.opt = Operation::div;
	return res;
}

mlib::Value mlib::Value::operator-() const
{
	Value res;
	static Value zero(0);
	res.a = &zero;
	res.b = this;
	res.opt = Operation::sub;
	return res;
}

mlib::Value mlib::Value::pow(const mlib::Value& b) const
{
	Value res;
	res.a = this;
	res.b = &b;
	res.opt = Operation::pow;
	return res;
}
mlib::Value mlib::Value::log(const mlib::Value& b) const
{
	Value res;
	res.a = this;
	res.b = &b;
	res.opt = Operation::log;
	return res;
}
mlib::Value mlib::Value::ln() const
{
	static Value e(M_E);
	return log(e);
}


mlib::Value mlib::Value::dvt(const mlib::Value& b) const
{
	Value res;
	res.a = this;
	res.b = &b;
	res.opt = Operation::dvt;
	return res;
}

/*非 const 运算*/
mlib::Value& mlib::Value::operator=(const mlib::Value& _b)
{
	static Value zero(0);
	a = &_b;
	b = &zero;
	opt = Operation::add;
	return *this;
}

/*以下功能仅限opt==null*/
bool mlib::Value::set(Math_F x)
{
	if (opt != Operation::null)return false;
	rv = x;
	return true;
}

bool mlib::Value::operator+=(Math_F x)
{
	if (opt != Operation::null)return false;
	rv += x;
	return true;
}
bool mlib::Value::operator-=(Math_F x)
{
	if (opt != Operation::null)return false;
	rv -= x;
	return true;
}
bool mlib::Value::operator*=(Math_F x)
{
	if (opt != Operation::null)return false;
	rv *= x;
	return true;
}
bool mlib::Value::operator/=(Math_F x)
{
	if (opt != Operation::null)return false;
	rv /= x;
	return true;
}

inline mlib::Value pow(const mlib::Value& a, const mlib::Value& b)
{
	return a.pow(b);
}
inline mlib::Value log(const mlib::Value& a, const mlib::Value& b)
{
	return a.log(b);
}
inline mlib::Value ln(const mlib::Value& a)
{
	return a.ln();
}
inline mlib::Value mlib::cvl(Math_F x)
{
	//找找看是否已经创建了
	for (size_t i = 0; i < const_value_objects.size(); i++)
	{
		if (const_value_objects[i].get() == x)return const_value_objects[i];
	}
	//创建新的
	const_value_objects.push_back(mlib::Value(x));
	return const_value_objects.back();
}

/*****************************************************************************
* IntEx
* 高精度整数
*****************************************************************************/

mlib::IntEx::IntEx(unsigned int x)
{
	while (x > 0)
	{
		arr.insert(arr.begin(), x % 10);
		x /= 10;
	}
}
mlib::IntEx::IntEx(int x)
{
	if (x < 0)
	{
		neg = true;
		x = -x;
	}
	while (x > 0)
	{
		arr.insert(arr.begin(), x % 10);
		x /= 10;
	}
}
mlib::IntEx::IntEx(const std::string& x)
{
	//合法性审查
	for (size_t i = 0; i < x.size(); i++)
	{
		if (x[i] < '0' || x[i] > '9')
		{
			if (i == 0 && (x[i] == '+' || x[i] == '-'))
			{
				continue;
			}
			return;
		}
	}
	//判断正负
	size_t l = 0;
	if (x[0] == '+')
	{
		neg = false;
		l = 1;
	}
	else if (x[0] == '-')
	{
		neg = true;
		l = 1;
	}
	//开始
	bool begin = false;
	for (size_t i = l; i < x.size(); i++)
	{
		if (!begin && x[i] != '0')begin = true;
		if (begin)arr.push_back(x[i] - '0');
	}
}

size_t mlib::IntEx::size() const
{
	return arr.size();
}

std::string mlib::IntEx::get_str(bool positive_sign) const
{
	std::string res;
	if (neg)
	{
		res += '-';
	}
	else if (positive_sign)
	{
		res += '+';
	}
	for (auto ch : arr)res += ch + '0';
	return res;
}

bool mlib::IntEx::operator==(const IntEx& b) const
{
	//从符号
	if (neg != b.neg)return false;
	//从长度
	if (size() != b.size())return false;
	//从数字
	for (size_t i = 0; i < size(); i++)
	{
		if (arr[i] != b.arr[i])return false;
	}
	return true;
}
bool mlib::IntEx::operator!=(const IntEx& b) const
{
	return !(*this == b);
}
bool mlib::IntEx::operator>(const IntEx& b) const
{
	//从符号
	if (!neg && b.neg)return true;
	else if (neg && !b.neg)return false;
	//从长度
	if (size() > b.size())
	{
		if (!neg)return true;
		else return false;
	}
	if (size() < b.size())
	{
		if (!neg)return false;
		else return true;
	}
	//从数字
	for (size_t i = 0; i < size(); i++)
	{
		if (arr[i] > b.arr[i])
		{
			if (!neg)return true;
			else return false;
		}
		if (arr[i] < b.arr[i])
		{
			if (!neg)return false;
			else return true;
		}
	}
	return false;
}
bool mlib::IntEx::operator>=(const IntEx& b) const
{
	return *this > b || *this == b;
}
bool mlib::IntEx::operator<(const IntEx& b) const
{
	return *this <= b && *this != b;
}
bool mlib::IntEx::operator<=(const IntEx& b) const
{
	return !(*this > b);
}

mlib::IntEx mlib::IntEx::operator-() const
{
	IntEx res = *this;
	res.neg = !neg;
	return res;
}

mlib::IntEx mlib::IntEx::operator+(const IntEx& b) const
{
	//同号
	if (neg == b.neg)
	{
		IntEx res;
		size_t max_size = size() > b.size() ? size() : b.size();
		res.arr.resize(max_size + 1);
		res.neg = neg;
		//开始运算
		for (size_t i = 1; i <= max_size; i++)
		{
			if (i <= size())
			{
				res.arr[max_size + 1 - i] += arr[size() - i];
			}
			if (i <= b.size())
			{
				res.arr[max_size + 1 - i] += b.arr[b.size() - i];
			}
			//进位
			if (res.arr[max_size + 1 - i] >= 10)
			{
				res.arr[max_size - i] += 1;
				res.arr[max_size + 1 - i] -= 10;
			}
		}
		//去0
		if (res.arr[0] == 0 && res.size() > 1)res.arr.erase(res.arr.begin());
		return res;
	}
	//异号
	else
	{
		if (neg)return b - (-*this);
		else return *this - (-b);
	}
}
mlib::IntEx mlib::IntEx::operator-(const IntEx& b) const
{
	//同号
	if (neg == b.neg)
	{
		//this < b
		if (*this < b)
		{
			IntEx res = b - *this;
			res.neg = true;
			return res;
		}
		//this >= b
		else
		{
			IntEx res, a = *this;
			size_t max_size = size() > b.size() ? size() : b.size();
			res.arr.resize(max_size);
			res.neg = neg;
			//开始运算
			for (size_t i = 1; i <= max_size; i++)
			{
				//借位
				if (a.arr[a.size() - i] < 0)
				{
					a.arr[a.size() - i] += 10;
					a.arr[a.size() - 1 - i] -= 1;
				}
				if (i <= a.size())
				{
					res.arr[max_size - i] += a.arr[a.size() - i];
				}
				if (i <= b.size())
				{
					//借位
					if (a.arr[a.size() - i] < b.arr[b.size() - i])
					{
						res.arr[max_size - i] += 10;
						a.arr[a.size() - 1 - i] -= 1;
					}

					res.arr[max_size - i] -= b.arr[b.size() - i];
				}
			}
			//去0
			while (res.arr[0] == 0 && res.size() > 1)res.arr.erase(res.arr.begin());
			return res;
		}
	}
	//异号
	else
	{
		if (neg)return -((-*this) + b);
		else return *this + b;
	}
}

#ifdef _IOSTREAM_
std::istream& operator>>(std::istream& is, mlib::IntEx& i)
{
	std::string str;
	is >> str;
	i = mlib::IntEx(str);
	return is;
}
std::ostream& operator<<(std::ostream& os, const mlib::IntEx& i)
{
	os << i.get_str();
	return os;
}
#endif