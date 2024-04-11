#pragma once
/*****************************************************************************
* mMath.h
* 数学
*
*****************************************************************************/
typedef double Math_F;

#include <corecrt_math_defines.h>//一些数学常量的宏定义
#include <cmath>
#include <Windows.h>
#include <vector>
#include <string>

//#include "mMath/num.h"	//多维数组
#include "mMath/geometry.h"	//几何
#include "mMath/algebra.h"	//代数
#include "mMath/value.h"	//求值链
#include "mMath/numEx.h"	//高精度数字

namespace mlib
{
	//映射
	inline Math_F Map(Math_F x, Math_F x_min, Math_F x_max, Math_F y_min, Math_F y_max)
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
	* 应用Verlet积分法
	*****************************************************************************/

	class Particle
	{
	private:
	//public:
		typedef double Math_F;
		Math_F x0, y0;	//上一帧的坐标
		Math_F x, y;	//坐标
		Vec F_all;	//当前质点受的所有力
	public:
		Math_F m;	//质量
		Vec v;	//速度

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

		//清空所有力
		void clr_force()
		{
			F_all.clear();
		}
		//添加外力
		void add_force(Vec F_new)
		{
			F_all += F_new;
		}
		//获取当前质点受力
		Vec get_force()
		{
			return F_all;
		}
		//作用所有力
		void wrk_force(Math_F tick)
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
	};
}