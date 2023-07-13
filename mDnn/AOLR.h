#pragma once
/*****************************************************************************
* AOLR.h
* ��������Ż�������ʧ���������򻯷���
* Activation, Optimizer, Loss, Regularizer
*****************************************************************************/

/*****************************************************************************
* �����
* Activation
*****************************************************************************/
enum class Activation
{
	undefined,
	linear,//���޼����
	//��������
	sigmoid,
	tanh,
	//ReLUs, �ֶ�
	relu,
	lrelu,
	elu,
	//����
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
* �Ż���(ѵ������)
* Optimizer
*****************************************************************************/
enum class Optimizer
{
	undefined,
	//�����ݶ��½���
	bgd,	//�����ݶ��½�(Batch Gradient Descent)
	sgd,	//����ݶ��½�(Stochastic Gradient Descent)
	//�߼�����
	adam,	//SGD + Adam(Adaptive Moment Estimation)
	adam_as	//Adam, �Զ�ֹͣѵ�������л���SGD(Adam Auto Stop)
};
//�ݶ�����(�����ݶȱ�ը)
static float g_limit = 10;	//���˹�С����Ȼ���кܴ�ĸ�������
//Adam �Ż����ĳ�����
static float adam_beta_1 = 0.9;
static float adam_beta_2 = 0.999;
static float adam_epsilon = 10e-8;

/*****************************************************************************
* ��ʧ����
* Loss
*****************************************************************************/
enum class Loss
{
	undefined,
	//�ع�
	mse,	//�������(mse-Mean Square Error��L2 loss)
	mae,	//ƽ���������(Mean Absolute Error��L1 loss)
	huber,	//���⻬���������(Smooth L1 Loss)
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
* ���򻯷���
* Regularizer
* ��ʵ��
*****************************************************************************/
enum class Regularizer
{
	undefined,
	none	//������
};