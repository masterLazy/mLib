#pragma once
/*****************************************************************************
* mMath.h
* ��ѧ
*
*****************************************************************************/
typedef double Math_F;
#define MMATH_H

#include <corecrt_math_defines.h>//һЩ��ѧ�����ĺ궨��
#include <cmath>
#include <Windows.h>
#include <vector>

#define _RST restrict(cpu)

#include "mMath/geometry.h"		//����
#include "mMath/ele_algebra.h"	//���ȴ���
#include "mMath/lin_algebra.h"	//���Դ���

namespace mlib
{
	/*****************************************************************************
	* Particle
	* �ʵ�
	*
	* Ӧ��Verlet���ַ�
	*****************************************************************************/
	class Particle
	{
	private:
		typedef double Math_F;
		Math_F x0, y0;	//��һ֡������
		Math_F x, y;	//����
		GVector F_all;	//��ǰ�ʵ��ܵ�������
	public:
		Math_F m;	//����
		GVector v;	//�ٶ�

		Particle() _RST
		{
			m = 0;
			x = y = 0;
			x0 = x;
			y0 = y;
		}
		Particle(Math_F _m, Math_F _x, Math_F _y) _RST
		{
			m = _m;
			x = _x;
			y = _y;
			x0 = x;
			y0 = y;
		}
		Particle(Math_F _m, Math_F _x, Math_F _y, GVector v0) _RST
		{
			m = _m;
			x = _x;
			y = _y;
			v = v0;
			x0 = x;
			y0 = y;
		}

		void Set(Math_F _x, Math_F _y) _RST
		{
			x = _x;
			y = _y;
			x0 = x;
			y0 = y;
		}
		void Set(Math_F _m, Math_F _x, Math_F _y) _RST
		{
			m = _m;
			x = _x;
			y = _y;
			x0 = x;
			y0 = y;
		}
		void Set(Math_F _m, Math_F _x, Math_F _y, GVector v0) _RST
		{
			m = _m;
			x = _x;
			y = _y;
			v = v0;
			x0 = x;
			y0 = y;
			v = v0;
		}

		void SetX(Math_F _x) _RST
		{
			x = _x;
			x0 = x;
		}
		void SetY(Math_F _y) _RST
		{
			y = _y;
			y0 = y;
		}

		Math_F GetX() const _RST
		{
			return x;
		}
		Math_F GetY() const _RST
		{
			return y;
		}

		Particle operator=(const Particle& par) _RST
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

		//�������
		void AddForce(GVector F_new) _RST
		{
			F_all += F_new;
		}
		//����������
		void AppForce(Math_F tick) _RST
		{
			//���µ�ǰ�ʵ������
			Math_F x_temp, y_temp;
			x_temp = x;
			y_temp = y;

			//������ٶ�
			GVector a;
			a = F_all / m;

			//Verlet���ַ� ����λ��
			x = 2 * x - x0 + pow(tick, 2) * a.split((Deg)0).len;
			y = 2 * y - y0 + pow(tick, 2) * a.split((Deg)90).len;

			//����"��һ֡������"
			x0 = x_temp;
			y0 = y_temp;

			//�����ٶ�
			v.set(x0, y0, x, y);

			//���������
			F_all.clear();
		}
	};
}