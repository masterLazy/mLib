#pragma once
/******************************************************
* geometry.h
* ����
*
******************************************************/


/******************************************************
* Angle
* ��
*
******************************************************/
class Angle
{
	Math_F fac;	//����,*this����ʾ�ĽǶ�=fac*M_PI rad
public:

	//����������
	class Deg
	{
	public:
		Math_F d;
		Deg(Math_F _d) _RST
		{
			d = _d;
		}
	};
	class Rad
	{
	public:
		Math_F r;
		Rad(Math_F _r) _RST
		{
			r = _r;
		}
	};

	Angle() _RST
	{
		fac = 0;
	}
	Angle(Deg deg) _RST
	{
		fac = deg.d / 180;
	}
	Angle(Rad rad) _RST
	{
		fac = rad.r / M_PI;
	}


	Math_F get_deg() const _RST
	{
		return fac * 180;
	}
	Math_F get_rad() const _RST
	{
		return fac * M_PI;
	}

	//��
	void sim() _RST
	{
		while (fac > 2)fac -= 2;
		while (fac < 0)fac += 2;
	}

	//��ȡ�����ǵĲ�(���Ӿ���ֵ)(С��ƽ��)
	static Angle dfc(Angle a, Angle b) _RST
	{
		Angle res;
		res.fac = a.fac - b.fac;
		//��������
		a.sim();
		b.sim();
		if (a.fac > 0 && a.fac < 0.5 && b.fac > 1.5 ||
			a.fac > 1.5 && b.fac > 0 && b.fac < 0.5)
		{
			res.fac = 2 + res.fac;
		}
		return res;
	}

	//����
	Angle operator=(const Angle& b) _RST
	{
		fac = b.fac;
		return *this;
	}
	//�෴����Ľ�
	Angle operator-() const _RST
	{
		Angle res;
		res.fac = fac + 1;
		return res;
	}

	Angle operator+(const Angle& b) const _RST
	{
		Angle res;
		res.fac = fac + b.fac;
		return res;
	}
	Angle operator-(const Angle& b) const _RST
	{
		Angle res;
		res.fac = fac - b.fac;
		return res;
	}

	Math_F operator*(const Angle& b) const _RST
	{
		return fac * b.fac * M_PI;
	}
	Math_F operator/(const Angle& b) const _RST
	{
		return fac / b.fac;
	}

	Angle operator*(const Math_F f) const _RST
	{
		Angle res;
		res.fac = fac * f;
		return res;
	}
	Angle operator/(const Math_F f) const _RST
	{
		Angle res;
		res.fac = fac / f;
		return res;
	}

	Angle friend operator*(const Math_F f, const Angle& b) _RST
	{
		Angle res;
		res.fac = f * b.fac;
		return res;
	}
	Angle friend operator/(const Math_F f, const Angle& b) _RST
	{
		Angle res;
		res.fac = f / b.fac;
		return res;
	}

	Angle operator+=(const Angle& b) _RST
	{
		*this = *this + b;
		return *this;
	}
	Angle operator-=(const Angle& b) _RST
	{
		*this = *this - b;
		return *this;
	}
	Angle operator*=(const Math_F f) _RST
	{
		*this = *this * f;
		return *this;
	}
	Angle operator/=(const Math_F f)
	{
		*this = *this / f;
		return *this;
	}

	//��ϵ����
	bool operator==(const Angle& b) const _RST
	{
		return fac == b.fac;
	}
	bool operator!=(const Angle& b) const _RST
	{
		return !(*this == b);
	}
	bool operator>(const Angle& b) const _RST
	{
		return fac > b.fac;
	}
	bool operator<(const Angle& b) const _RST
	{
		return fac < b.fac;
	}
	bool operator>=(const Angle& b) const _RST
	{
		return !(*this < b);
	}
	bool operator<=(const Angle& b) const _RST
	{
		return !(*this > b);
	}
};
typedef Angle::Deg Deg;
typedef Angle::Rad Rad;

//���Ǻ���֧��
Math_F sin(Angle a) _RST
{
	return sin(a.get_rad());
}
Math_F cos(Angle a) _RST
{
	return cos(a.get_rad());
}
Math_F tan(Angle a) _RST
{
	return tan(a.get_rad());
}
Angle arcsin(Math_F f) _RST
{
	return (Rad)asin(f);
}
Angle arccos(Math_F f) _RST
{
	return (Rad)acos(f);
}
Angle arctan(Math_F f) _RST
{
	return (Rad)atan(f);
}


/******************************************************
* Vector
* �������������壩
*
******************************************************/
class Vector
{
private:
	void sim() _RST
	{
		if (len < 0)
		{
			theta = -theta;
			len = -len;
		}
		theta.sim();
	}
public:
	Angle theta;	//����
	Math_F len;		//����(ģ)

	Vector() _RST
	{
		theta = (Deg)0;
		len = 0;
	}
	Vector(Angle _theta, Math_F _len) _RST
	{
		theta = _theta;
		len = _len;
		sim();
	}
	Vector(Math_F x, Math_F y) _RST
	{
		len = sqrt(pow(x, 2) + pow(y, 2));
		if (len != 0)theta = arcsin(y / len);
		else theta = (Deg)0;
		//��������
		if (x < 0)theta = (Angle)(Deg)180 - theta;

		sim();
	}
	Vector(Math_F x0, Math_F y0, Math_F x1, Math_F y1) _RST
	{
		*this = Vector(x1 - x0, y1 - y0);
	}

	void set(Angle _theta, Math_F _len) _RST
	{
		theta = _theta;
		len = _len;
		sim();
	}
	void set(Math_F x, Math_F y) _RST
	{
		len = sqrt(pow(x, 2) + pow(y, 2));
		if (len != 0)theta = arcsin(y / len);
		else theta = (Deg)0;
		//��������
		if (x < 0)theta = (Angle)(Deg)180 - theta;

		sim();
	}
	void set(Math_F x0, Math_F y0, Math_F x1, Math_F y1) _RST
	{
		set(x1 - x0, y1 - y0);
	}

	void clear() _RST
	{
		theta = (Deg)0;
		len = 0;
	}

	Math_F get_x() const _RST
	{
		return len * cos(theta);
	}
	Math_F get_y() const _RST
	{
		return len * sin(theta);
	}

	//����
	Vector operator=(const Vector& b) _RST
	{
		theta = b.theta;
		len = b.len;
		return *this;
	}
	Vector operator-() const _RST
	{
		return Vector(theta + (Deg)180, len);
	}

	Vector operator+(const Vector& b) const _RST
	{
		return Vector(get_x() + b.get_x(), get_y() + b.get_y());
	}
	Vector operator-(const Vector& b) const _RST
	{
		return Vector(get_x() - b.get_x(), get_y() - b.get_y());
	}
	Vector operator*(Math_F f) const _RST
	{
		return Vector(f < 0 ? -theta : theta, len* fabs(f));
	}
	Vector operator/(Math_F f) const _RST
	{
		return Vector(f < 0 ? -theta : theta, len / fabs(f));
	}

	Vector friend operator*(const Math_F f, const Vector& v) _RST
	{
		return v * f;
	}

	Vector operator+=(const Vector& b) _RST
	{
		*this = *this + b;
		return *this;
	}
	Vector operator-=(const Vector& b) _RST
	{
		*this = *this - b;
		return *this;
	}
	Vector operator*=(const Math_F f) _RST
	{
		*this = *this * f;
		return *this;
	}
	Vector operator/=(const Math_F f) _RST
	{
		*this = *this / f;
		return *this;
	}

	//��ϵ����
	bool operator==(const Vector& b) const _RST
	{
		return theta == b.theta && len == b.len;
	}
	bool operator!=(const Vector& b) const _RST
	{
		return !(*this == b);
	}
	bool operator>(const Vector& b) const _RST
	{
		return len > b.len;
	}
	bool operator<(const Vector& b) const _RST
	{
		return len < b.len;
	}
	bool operator>=(const Vector& b) const _RST
	{
		return !(*this < b);
	}
	bool operator<=(const Vector& b) const _RST
	{
		return !(*this > b);
	}

	//����
	void split(Angle thetaA, Angle thetaB, Vector* a, Vector* b) _RST
	{
		Angle alpha = theta - thetaA;
		Angle beta = thetaB - theta;
		a->set(thetaA, len * sin(beta) / sin(alpha + beta));
		b->set(thetaB, len * sin(alpha) / sin(alpha + beta));
	}
	//��ֱ����
	Vector split(Angle thetaV) _RST
	{
		Angle alpha = theta - thetaV;
		Vector res(thetaV, len * cos(alpha));
		//ǧ��ȷ������һ��,��Sim()
		if (res.theta != thetaV)
		{
			res.theta = -res.theta;
			res.len = -res.len;
		}
		return res;
	}

	//����
#ifdef EW_NOCLOSE	//easyx.h�еĺ궨��
	void Draw(Mathe_F x, Mathe_F y, COLORREF color = RGB(255, 255, 255), Mathe_F zoom = 1)
	{
		setlinecolor(color);
		setfillcolor(color);
		setlinestyle(PS_SOLID, 2);

		POINT pt;
		pt = GetP(x, y, theta, len * zoom);
		line(x, y, pt.x, pt.y);

		setlinestyle(PS_SOLID, 1);
		POINT pts[3];
		pts[1] = GetP(x, y, theta, len * zoom);
		pts[0] = GetP(pts[1], theta + (Deg)(180 + 30), min(20, len * zoom / 2));
		pts[2] = GetP(pts[1], theta + (Deg)(180 - 30), min(20, len * zoom / 2));
		fillpolygon(pts, 3);
	}
#endif
};