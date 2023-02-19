#pragma once
/******************************************************
* mNetwork.h
* 神经网络
*
******************************************************/
typedef float Net_F;

#include <cmath>
#include <corecrt_math_defines.h>
#include <vector>
#include <iostream>
#include <amp.h>
#include <amp_math.h>
using namespace concurrency::fast_math;
using namespace std;

//激活函数
//fun
#define SIGMOID	0
#define TANH	1
#define RELU	2
#define LRELU	3
Net_F Act(Net_F x, int fun)
{
	switch (fun)
	{
	case SIGMOID:return 1 / (1 + pow(M_E, -x));
	case TANH:return tanh(x);
	case RELU:return x <= 0 ? 0 : x;
	case LRELU:return x <= 0 ? 0.01 * x : x;
	default:return x;
	}
}
Net_F Act(Net_F x, int fun) restrict(amp)
{
	switch (fun)
	{
	case SIGMOID:return 1 / (1 + pow(M_E, -x));
	case TANH:return tanh(x);
	case RELU:return x <= 0 ? 0 : x;
	case LRELU:return x <= 0 ? 0.01 * x : x;
	default:return x;
	}
}
Net_F dAct(Net_F x, int fun)
{
	switch (fun)
	{
	case SIGMOID:return Act(x, 0) * (1 - Act(x, 0));
	case TANH:return 1 - pow(tanh(x), 2);
	case RELU:return x <= 0 ? 0 : 1;
	case LRELU:return x <= 0 ? 0.01 : 1;
	default:return 1;
	}
}
Net_F dAct(Net_F x, int fun) restrict(amp)
{
	switch (fun)
	{
	case SIGMOID:return Act(x, 0) * (1 - Act(x, 0));
	case TANH:return 1 - pow(tanh(x), 2);
	case RELU:return x <= 0 ? 0 : 1;
	case LRELU:return x <= 0 ? 0.01 : 1;
	default:return 1;
	}
}


#include "mNetwork/fnn.h"	//全连接层