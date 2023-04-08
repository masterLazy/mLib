#pragma once
/*****************************************************************************
* mMath.h
* 数学
*
*****************************************************************************/
typedef double Math_F;
#define MMATH_H

#include <corecrt_math_defines.h>//一些数学常量的宏定义
#include <cmath>
#include <Windows.h>
#include <vector>

#define _RST restrict(cpu)

#include "mMath/geometry.h"		//几何
#include "mMath/ele_algebra.h"	//初等代数
#include "mMath/lin_algebra.h"	//线性代数

namespace mlib
{
	/*****************************************************************************
	* Particle
	* 质点
	*
	* 应用Verlet积分法
	*****************************************************************************/
	class Particle
	{
	private:
		typedef double Math_F;
		Math_F x0, y0;	//上一帧的坐标
		Math_F x, y;	//坐标
		GVector F_all;	//当前质点受的所有力
	public:
		Math_F m;	//质量
		GVector v;	//速度

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

		//添加外力
		void AddForce(GVector F_new) _RST
		{
			F_all += F_new;
		}
		//作用所有力
		void AppForce(Math_F tick) _RST
		{
			//存下当前质点的坐标
			Math_F x_temp, y_temp;
			x_temp = x;
			y_temp = y;

			//计算加速度
			GVector a;
			a = F_all / m;

			//Verlet积分法 计算位移
			x = 2 * x - x0 + pow(tick, 2) * a.split((Deg)0).len;
			y = 2 * y - y0 + pow(tick, 2) * a.split((Deg)90).len;

			//更新"上一帧的坐标"
			x0 = x_temp;
			y0 = y_temp;

			//计算速度
			v.set(x0, y0, x, y);

			//清空所受力
			F_all.clear();
		}
	};
}