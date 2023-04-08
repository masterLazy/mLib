#pragma once
/*****************************************************************************
* mNetwork.h
* 神经网络
*
*****************************************************************************/

#include <cmath>
#include <corecrt_math_defines.h>
#include <vector>
#include <iostream>
#include "mMath.h"
#include "mFile.h"
using namespace std;

namespace mlib
{
	//激活函数
	//fun
#define SIGMOID	0
#define TANH	1
#define RELU	2
#define LRELU	3
#define ELU	4

	Math_F Act(Math_F x, int fun)
	{
		switch (fun)
		{
		case SIGMOID:return 1 / (1 + pow(M_E, -x));
		case TANH:return tanh(x);
		case RELU:return x <= 0 ? 0 : x;
		case LRELU:return x <= 0 ? 0.01 * x : x;
		case ELU:return x <= 0 ? pow(M_E, x) - 1 : x;
		default:return x;
		}
	}

	Math_F dAct(Math_F x, int fun)
	{
		switch (fun)
		{
		case SIGMOID:return Act(x, 0) * (1 - Act(x, 0));
		case TANH:return 1 - pow(tanh(x), 2);
		case RELU:return x <= 0 ? 0 : 1;
		case LRELU:return x <= 0 ? 0.01 : 1;
		case ELU:return x <= 0 ? pow(M_E, x) : 1;
		default:return 1;
		}
	}

	//随机函数
	Math_F _Random(Math_F useless = 0)
	{
		//return 1;
		return (Math_F)rand() / RAND_MAX;
	}
}

#include "mNetwork/fnn.h"	//全连接神经网络