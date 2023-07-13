#pragma once
/*****************************************************************************
* AOLR.h
* 激活函数、优化器、损失函数、正则化方法
* Activation, Optimizer, Loss, Regularizer
*****************************************************************************/

/*****************************************************************************
* 激活函数
* Activation
*****************************************************************************/
enum class Activation
{
	undefined,
	linear,//即无激活函数
	//有限区间
	sigmoid,
	tanh,
	//ReLUs, 分段
	relu,
	lrelu,
	elu,
	//其他
	softplus
};
inline Dnn_F Act(Dnn_F x, mlib::Activation act)
{
	switch (act)
	{
	case mlib::Activation::linear:
	{
		return x;
	}
	case mlib::Activation::sigmoid:
	{
		return 1 / (1 + exp(-x));
	}
	case mlib::Activation::tanh:
	{
		Dnn_F a = exp(x);
		Dnn_F b = exp(-x);
		return (a - b) / (a + b);
	}
	case mlib::Activation::relu:
	{
		if (x >= 0)return x;
		return 0;
	}
	case mlib::Activation::lrelu:
	{
		if (x >= 0)return x;
		return 0.01 * x;
	}
	case mlib::Activation::elu:
	{
		if (x >= 0)return x;
		return exp(x) - 1;
	}
	case mlib::Activation::softplus:
	{
		return ::log(exp(x) + 1);
	}
	}
	return 0;
}
inline Dnn_F dAct(Dnn_F x, mlib::Activation act)
{
	switch (act)
	{
	case mlib::Activation::linear:
	{
		return 1;
	}
	case mlib::Activation::sigmoid:
	{
		Dnn_F temp = mlib::Act(x, act);
		return temp * (1 - temp);
	}
	case mlib::Activation::tanh:
	{
		return 1 - ::pow(mlib::Act(x, act), 2);
	}
	case mlib::Activation::relu:
	{
		if (x >= 0)return 1;
		return 0;
	}
	case mlib::Activation::lrelu:
	{
		if (x >= 0)return 1;
		return 0.01;
	}
	case mlib::Activation::elu:
	{
		if (x >= 0)return 1;
		return exp(x);
	}
	case mlib::Activation::softplus:
	{
		return 1 / (1 + exp(-x));
	}
	}
	return 0;
}

/*****************************************************************************
* 优化器(训练方法)
* Optimizer
*****************************************************************************/
enum class Optimizer
{
	undefined,
	//基本梯度下降法
	bgd,	//批量梯度下降(Batch Gradient Descent)
	sgd,	//随机梯度下降(Stochastic Gradient Descent)
	//高级方法
	adam,	//SGD + Adam(Adaptive Moment Estimation)
	adam_as	//Adam, 自动停止训练，并切换至SGD(Adam Auto Stop)
};
//梯度限制(避免梯度爆炸)
static float g_limit = 10;	//不宜过小，不然会有很大的负面作用
//Adam 优化器的超参数
static float adam_beta_1 = 0.9;
static float adam_beta_2 = 0.999;
static float adam_epsilon = 10e-8;

/*****************************************************************************
* 损失函数
* Loss
*****************************************************************************/
enum class Loss
{
	undefined,
	//回归
	mse,	//均方误差(mse-Mean Square Error，L2 loss)
	mae,	//平均绝对误差(Mean Absolute Error，L1 loss)
	huber,	//“光滑”均方误差(Smooth L1 Loss)
};
inline Dnn_F Lss(Dnn_F a, Dnn_F y, mlib::Loss loss)
{
	switch (loss)
	{
	case mlib::Loss::mae:
		return abs(a - y);
	case mlib::Loss::mse:
		return ::pow(a - y, 2);
	case mlib::Loss::huber:
		if (abs(a - y) < 1) return ::pow(a - y, 2) / 2;
		else return abs(a - y) - 1 / 2;
	}
	return 0;
}
inline Dnn_F dLss(Dnn_F a, Dnn_F y, mlib::Loss loss)
{
	switch (loss)
	{
	case mlib::Loss::mae:
		if (a - y < 0)return -1;
		else if (a - y == 0)return 0;
		else return 1;
	case mlib::Loss::mse:
		return 2 * (a - y);
	case mlib::Loss::huber:
		if (abs(a - y) < 1) return (a - y);
		else if (a - y < 0)return -1;
		else return 1;
	}
	return 0;
}

/*****************************************************************************
* 正则化方法
* Regularizer
* 待实现
*****************************************************************************/
enum class Regularizer
{
	undefined,
	none	//无正则化
};