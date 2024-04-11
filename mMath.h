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

//#include "mMath/num.h"		//多维数组
#include "mMath/geometry.h"	//几何
#include "mMath/algebra.h"	//代数
#include "mMath/value.h"	//求值链
#include "mMath/numEx.h"	//高精度数字

namespace mlib
{
	//映射
	inline Math_F Map(Math_F x, Math_F x_min, Math_F x_max, Math_F y_min, Math_F y_max);

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

		//清空所有力
		void clr_force();
		//添加外力
		void add_force(Vec F_new);
		//获取当前质点受力
		Vec get_force();
		//作用所有力
		void wrk_force(Math_F tick);
	};
}

#include "mMath.hpp"