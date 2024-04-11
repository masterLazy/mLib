#pragma once
/*****************************************************************************
* mMath.h
* ��ѧ
*
*****************************************************************************/
typedef double Math_F;

#include <corecrt_math_defines.h>//һЩ��ѧ�����ĺ궨��
#include <cmath>
#include <Windows.h>
#include <vector>
#include <string>

//#include "mMath/num.h"	//��ά����
#include "mMath/geometry.h"	//����
#include "mMath/algebra.h"	//����
#include "mMath/value.h"	//��ֵ��
#include "mMath/numEx.h"	//�߾�������

namespace mlib
{
	//ӳ��
	inline Math_F Map(Math_F x, Math_F x_min, Math_F x_max, Math_F y_min, Math_F y_max)
	{
		//��һ��
		Math_F one = (x - x_min) / (x_max - x_min);
		//ӳ��
		Math_F m_map = one * (y_max - y_min) + y_min;
		//ȷ����Χ
		//if (m_map < y_min)return y_min;
		//else if (m_map > y_max)return y_max;
		return m_map;
	}

	/*****************************************************************************
	* Particle
	* �ʵ�
	* Ӧ��Verlet���ַ�
	*****************************************************************************/

	class Particle
	{
	private:
	//public:
		typedef double Math_F;
		Math_F x0, y0;	//��һ֡������
		Math_F x, y;	//����
		Vec F_all;	//��ǰ�ʵ��ܵ�������
	public:
		Math_F m;	//����
		Vec v;	//�ٶ�

		Particle()
		{
			m = 0;
			x = y = 0;
			x0 = x;
			y0 = y;
		}
		Particle(Math_F _m, Math_F _x, Math_F _y)
		{
			m = _m;
			x = _x;
			y = _y;
			x0 = x;
			y0 = y;
		}
		Particle(Math_F _m, Math_F _x, Math_F _y, Vector v0)
		{
			m = _m;
			x = _x;
			y = _y;
			v = v0;
			x0 = x;
			y0 = y;
		}

		void set(Math_F _x, Math_F _y)
		{
			x = _x;
			y = _y;
			x0 = x;
			y0 = y;
		}
		void set(Math_F _m, Math_F _x, Math_F _y)
		{
			m = _m;
			x = _x;
			y = _y;
			x0 = x;
			y0 = y;
		}
		void set(Math_F _m, Math_F _x, Math_F _y, Vector v0)
		{
			m = _m;
			x = _x;
			y = _y;
			v = v0;
			x0 = x;
			y0 = y;
			v = v0;
		}

		void set_x(Math_F _x)
		{
			x = _x;
			x0 = x;
		}
		void set_y(Math_F _y)
		{
			y = _y;
			y0 = y;
		}

		Math_F get_x() const
		{
			return x;
		}
		Math_F get_y() const
		{
			return y;
		}

		Particle operator=(const Particle& par)
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

		//���������
		void clr_force()
		{
			F_all.clear();
		}
		//�������
		void add_force(Vec F_new)
		{
			F_all += F_new;
		}
		//��ȡ��ǰ�ʵ�����
		Vec get_force()
		{
			return F_all;
		}
		//����������
		void wrk_force(Math_F tick)
		{
			//���µ�ǰ�ʵ������
			Math_F x_temp, y_temp;
			x_temp = x;
			y_temp = y;

			//������ٶ�
			Vector a;
			a = F_all / m;

			//Verlet���ַ� ����λ��
			x = 2 * x - x0 + ::pow(tick, 2) * a.split((Deg)0).len;
			y = 2 * y - y0 + ::pow(tick, 2) * a.split((Deg)90).len;

			//����"��һ֡������"
			x0 = x_temp;
			y0 = y_temp;

			//�����ٶ�
			v = { x0, y0, x, y };

			//���������
			F_all.clear();
		}
	};
}