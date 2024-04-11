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

//#include "mMath/num.h"		//��ά����
#include "mMath/geometry.h"	//����
#include "mMath/algebra.h"	//����
#include "mMath/value.h"	//��ֵ��
#include "mMath/numEx.h"	//�߾�������

namespace mlib
{
	//ӳ��
	inline Math_F Map(Math_F x, Math_F x_min, Math_F x_max, Math_F y_min, Math_F y_max);

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

		Particle();
		Particle(Math_F _m, Math_F _x, Math_F _y);
		Particle(Math_F _m, Math_F _x, Math_F _y, Vector v0);

		void set(Math_F _x, Math_F _y);
		void set(Math_F _m, Math_F _x, Math_F _y);
		void set(Math_F _m, Math_F _x, Math_F _y, Vector v0);

		void set_x(Math_F _x);
		void set_y(Math_F _y);

		Math_F get_x() const;
		Math_F get_y() const;

		Particle operator=(const Particle& par);

		//���������
		void clr_force();
		//�������
		void add_force(Vec F_new);
		//��ȡ��ǰ�ʵ�����
		Vec get_force();
		//����������
		void wrk_force(Math_F tick);
	};
}

#include "mMath.hpp"