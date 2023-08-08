/*****************************************************************************
* mDnn.cpp
* ������
*
*****************************************************************************/

#include "mDnn.h"

#include <iostream>	//FOR DEBUG

using namespace mlib;

/*��*/

/*****************************************************************************
* DnnLayer
* ������� ����
*****************************************************************************/
Dnn_F DnnLayer::Act(Dnn_F x)
{
	return ::Act(x, act);
}
Dnn_F DnnLayer::dAct(Dnn_F x)
{
	return ::dAct(x, act);
}
Shape DnnLayer::get_input_shape() const
{
	return in_shape;
}
Shape DnnLayer::get_output_shape() const
{
	return out_shape;
}

using namespace mlib::layers;

/*****************************************************************************
* Input
* �����
*****************************************************************************/
Input::Input(Shape shape)
{
	in_shape = shape;
	out_shape = shape;

	in_size = SizeOfShape(shape);
	act = Activation::linear;

	a.resize(in_size);
}
void Input::set(const Array_F& values)
{
	//����쳣
	if (values.size() != in_size)throw_error(DnnLayerError::input_setting);

	a = values;
}
Array_F Input::predict()
{
	return a;
}

/*****************************************************************************
* Trans
* ת���� (ת�� Shape)
*****************************************************************************/
Trans::Trans(Shape input_shape, Shape output_shape)
{
	if (output_shape.empty())
	{
		output_shape = { SizeOfShape(input_shape) };
	}
	//����쳣
	if (SizeOfShape(input_shape) != SizeOfShape(output_shape))throw_error(DnnLayerError::trans_creating);
	in_shape = input_shape;
	out_shape = output_shape;
}

/*****************************************************************************
* Dense
* ȫ�����������
*****************************************************************************/
Dense::Dense(Shape input_shape, Activation activation, unsigned _units)
{
	//����쳣
	if (input_shape.size() != 1 || SizeOfShape(input_shape) == 0)throw_error(DnnLayerError::creating_shape);
	if (_units == 0 || activation == Activation::undefined)throw_error(DnnLayerError::creating_invalid);

	//��ʼ���ṹ����
	in_size = SizeOfShape(input_shape);
	units = _units;

	in_shape.resize(1);
	in_shape = input_shape;
	out_shape.resize(1);
	out_shape[0] = units;

	act = activation;

	//��ʼ������
	w.resize(in_size * units);
	b.resize(units);
	//�����ʼ��
	for (unsigned i = 0; i < w.size(); i++)w[i] = (Dnn_F)rand() / RAND_MAX * 1 - 0.5;
	for (unsigned i = 0; i < b.size(); i++)b[i] = (Dnn_F)rand() / RAND_MAX * 1 - 0.5;
	w_g.resize(w.size());
	w_m.resize(w.size());
	w_v.resize(w.size());
	b_g.resize(b.size());
	b_m.resize(b.size());
	b_v.resize(b.size());
	t = 0;
}
Dense::Info Dense::get_info()
{
	return { units };
}
Array_F Dense::predict(const Array_F& input)
{
	//����쳣
	if (input.size() != in_size)throw_error(DnnLayerError::predicting_size);

	//��ղ���ʼ��
	z.clear();
	z.resize(SizeOfShape(out_shape));
	a.clear();
	a.resize(SizeOfShape(out_shape));

	last_in = input;
	for (unsigned i = 0; i < units; i++)
	{
		for (unsigned j = 0; j < in_size; j++)
		{
			z[i] += input[j] * w[i * in_size + j];
		}
		z[i] += b[i];
		a[i] = Act(z[i]);
	}
	return a;
}
Array_F Dense::train(const Array_F& dvt, Regularizer reg)
{
	//����쳣
	if (dvt.size() != units)throw_error(DnnLayerError::training_dvt_size);

	Array_F dC_dla(in_size);//����һ���ƫ����
	for (unsigned i = 0; i < units; i++)
	{
		//dC/da = da/dz * dC/da
		Dnn_F dC_da = dAct(z[i]) * dvt[i];

		b_g[i] += dC_da;
		for (unsigned j = 0; j < in_size; j++)
		{
			w_g[i * in_size + j] += last_in[j] * dC_da;
			dC_dla[j] += w[i * in_size + j] * dC_da;
		}
	}
	train_time++;
	return dC_dla;
}
Array_F Dense::train_last(const Array_F& output, Loss loss, Regularizer reg)
{
	//����쳣
	if (output.size() != units)throw_error(DnnLayerError::training_out_size);

	Array_F dC_dla(in_size);//����һ���ƫ����
	for (unsigned i = 0; i < units; i++)
	{
		//dC/da = da/dz * dC/da
		Dnn_F dC_da = dAct(z[i]) * dLss(a[i], output[i], loss);

		b_g[i] += dC_da;
		for (unsigned j = 0; j < in_size; j++)
		{
			w_g[i * in_size + j] += last_in[j] * dC_da;
			dC_dla[j] += w[i * in_size + j] * dC_da;
		}
	}
	train_time++;
	return dC_dla;
}
void Dense::fit(Dnn_F lr, Optimizer opt)
{
	t++;//һ��Ҫ���ڿ�ͷ
	if (train_time != 0)
	{
		if (opt == Optimizer::bgd || opt == Optimizer::sgd)
		{
			for (unsigned i = 0; i < b.size(); i++)
			{
				//�ݶ�����
				if (b_g[i] > g_limit)b_g[i] = g_limit;
				else if (b_g[i] < -g_limit)b_g[i] = -g_limit;

				b[i] -= b_g[i] / train_time * lr;
			}
			for (unsigned i = 0; i < w.size(); i++)
			{
				//�ݶ�����
				if (w_g[i] > g_limit)w_g[i] = g_limit;
				else if (w_g[i] < -g_limit)w_g[i] = -g_limit;

				w[i] -= w_g[i] / train_time * lr;
			}
		}
		else if (opt == Optimizer::adam || opt == Optimizer::adam_as)
		{
			float m_hat, v_hat;
			for (unsigned i = 0; i < b.size(); i++)
			{
				//�ݶ�����
				if (b_g[i] > g_limit)b_g[i] = g_limit;
				else if (b_g[i] < -g_limit)b_g[i] = -g_limit;

				b_m[i] = adam_beta_1 * b_m[i] + (1 - adam_beta_1) * b_g[i];
				b_v[i] = adam_beta_2 * b_v[i] + (1 - adam_beta_2) * pow(b_g[i], 2);
				m_hat = b_m[i] / (1 - pow(adam_beta_1, t));
				v_hat = b_v[i] / (1 - pow(adam_beta_2, t));
				b[i] -= lr / (sqrt(v_hat) + adam_epsilon) * m_hat;
			}
			for (unsigned i = 0; i < w.size(); i++)
			{
				//�ݶ�����
				if (w_g[i] > g_limit)w_g[i] = g_limit;
				else if (w_g[i] < -g_limit)w_g[i] = -g_limit;

				w_m[i] = adam_beta_1 * w_m[i] + (1 - adam_beta_1) * w_g[i];
				w_v[i] = adam_beta_2 * w_v[i] + (1 - adam_beta_2) * pow(w_g[i], 2);
				m_hat = w_m[i] / (1 - pow(adam_beta_1, t));
				v_hat = w_v[i] / (1 - pow(adam_beta_2, t));
				w[i] -= lr / (sqrt(v_hat) + adam_epsilon) * m_hat;
			}
		}

		//��ղ���ʼ��
		w_g.clear();
		w_g.resize(w.size());
		b_g.clear();
		b_g.resize(b.size());
		train_time = 0;
	}
}

/*****************************************************************************
* Conv2d
* ��ά����������
*****************************************************************************/
Conv2d::Conv2d(Shape input_shape,
	mlib::Activation activation,
	unsigned kernels,
	unsigned kernel_w, unsigned kernel_h,
	unsigned padding_w, unsigned padding_h,
	unsigned stride_w, unsigned stride_h)
{
	//Ԥ����һ��
	if (input_shape.size() == 2)input_shape.push_back(1);
	//����쳣
	if (input_shape.size() != 3 || SizeOfShape(input_shape) == 0)throw_error(DnnLayerError::creating_shape);
	if (kernels == 0 || kernel_w == 0 || kernel_h == 0 ||
		stride_w == 0 || stride_h == 0 || activation == Activation::undefined ||
		kernel_w > input_shape[0] || kernel_h > input_shape[1] ||
		stride_w > input_shape[0] || stride_h > input_shape[1])
	{
		throw_error(DnnLayerError::creating_invalid);
	}

	//��ʼ���ṹ����
	in_size = SizeOfShape(input_shape);
	in_dim = input_shape[2];
	kns = kernels;
	kn_w = kernel_w;
	kn_h = kernel_h;
	if (padding_h == -1 || padding_w == -1)
	{
		if (kernel_w % 2 == 0 || kernel_h % 2 == 0)throw(DnnLayerError::conv2d_padding);
		pad_w = (stride_w * input_shape[0] - stride_w - input_shape[0] + kn_w) / 2;
		pad_h = (stride_h * input_shape[1] - stride_h - input_shape[1] + kn_h) / 2;
	}
	else
	{
		pad_w = padding_w;
		pad_h = padding_h;
	}
	str_w = stride_w;
	str_h = stride_h;

	in_shape = input_shape;
	out_shape.resize(3);
	out_shape[0] = (in_shape[0] - kn_w + 2 * pad_w) / str_w + 1;
	out_shape[1] = (in_shape[1] - kn_h + 2 * pad_h) / str_h + 1;
	out_shape[2] = kns;

	act = activation;

	//��ʼ������
	w.resize(kns * kn_w * kn_h * in_dim);
	b.resize(kns);
	//�����ʼ��
	for (unsigned i = 0; i < w.size(); i++)w[i] = (Dnn_F)rand() / RAND_MAX * 1 - 0.5;
	for (unsigned i = 0; i < b.size(); i++)b[i] = (Dnn_F)rand() / RAND_MAX * 1 - 0.5;
	w_g.resize(w.size());
	w_m.resize(w.size());
	w_v.resize(w.size());
	b_g.resize(b.size());
	b_m.resize(b.size());
	b_v.resize(b.size());
	t = 0;
}
Conv2d::Info Conv2d::get_info()
{
	return
	{
		kns,
		kn_w,kn_h,
		pad_w,pad_h,
		str_w,str_h
	};
}
Array_F Conv2d::predict(const Array_F& input)
{
	//����쳣
	if (input.size() != in_size)throw_error(DnnLayerError::predicting_size);

	//��ղ���ʼ��
	z.clear();
	z.resize(SizeOfShape(out_shape));
	a.clear();
	a.resize(SizeOfShape(out_shape));

	//pad�������Ŀ��
	unsigned wid = in_shape[0] + 2 * pad_w;
	unsigned hei = in_shape[1] + 2 * pad_h;
	//���padding
	std::vector<float> in(wid * hei * in_dim);
	if (pad_w + pad_h != 0)
	{
		for (unsigned x = 0; x < in_shape[0]; x++)
		{
			for (unsigned y = 0; y < in_shape[1]; y++)
			{
				for (unsigned z = 0; z < in_dim; z++)
				{
					in[(x + pad_w) * hei * in_dim + (y + pad_h) * in_dim + z] =
						input[x * in_shape[1] * in_dim + y * in_dim + z];
				}
			}
		}
	}
	else
	{
		in = input;
	}

	last_in = in;
	//���Ƶ�Ƕ��
	for (unsigned i = 0; i < kns; i++)
	{
		//x0, y0: �����(���Ͻ�)��λ��
		for (unsigned x0 = 0; x0 <= wid - kn_w; x0 += str_w)
		{
			for (unsigned y0 = 0; y0 <= hei - kn_h; y0 += str_h)
			{
				//��ǰz,a���±�
				unsigned temp = (x0 / str_w) * out_shape[1] * kns + (y0 / str_h) * kns + i;
				if (temp >= SizeOfShape(out_shape))continue;//Ӳ�˽��������
				//x1, y1: ���ڼ����λ��(����ھ����)
				for (unsigned x1 = 0; x1 < kn_w; x1++)
				{
					for (unsigned y1 = 0; y1 < kn_h; y1++)
					{
						//z0: �����ͨ����
						for (unsigned z0 = 0; z0 < in_dim; z0++)
						{
							//��ǰw���±�
							unsigned wi = i * kn_w * kn_h * in_dim + x1 * kn_h * in_dim + y1 * in_dim + z0;
							z[temp] +=
								in[(x0 + x1) * hei * in_dim + (y0 + y1) * in_dim + z0] * w[wi];
						}
					}
				}
				z[temp] += b[i];
				a[temp] = Act(z[temp]);
			}
		}
	}
	return a;
}
Array_F Conv2d::train(const Array_F& dvt, Regularizer reg)
{
	//����쳣
	if (dvt.size() != SizeOfShape(out_shape))throw_error(DnnLayerError::training_dvt_size);
	if (last_in.empty())throw_error(DnnLayerError::trainning_predict);

	Array_F dC_dla(in_size);//����һ���ƫ����

	//pad�������Ŀ��
	unsigned wid = in_shape[0] + 2 * pad_w;
	unsigned hei = in_shape[1] + 2 * pad_h;

	//���Ƶ�Ƕ��
	for (unsigned i = 0; i < kns; i++)
	{
		//x0, y0: �����(���Ͻ�)��λ��
		for (unsigned x0 = 0; x0 <= wid - kn_w; x0 += str_w)
		{
			for (unsigned y0 = 0; y0 <= hei - kn_h; y0 += str_h)
			{
				//��ǰz,a,dvt���±�
				unsigned temp = (x0 / str_w) * out_shape[1] * kns + (y0 / str_h) * kns + i;
				if (temp >= SizeOfShape(out_shape))continue;//Ӳ�˽��������
				//dC/da = da/dz * dC/da
				Dnn_F dC_da = dAct(z[temp]) * dvt[temp];
				b_g[i] += dC_da;

				//x1, y1: ���ڼ����λ��(����ھ����)
				for (unsigned x1 = 0; x1 < kn_w; x1++)
				{
					for (unsigned y1 = 0; y1 < kn_h; y1++)
					{
						//z0: �����ͨ����
						for (unsigned z0 = 0; z0 < in_dim; z0++)
						{
							//��ǰw���±�
							unsigned wi = i * kn_w * kn_h * in_dim + x1 * kn_h * in_dim + y1 * in_dim + z0;
							w_g[wi] += last_in[(x0 + x1) * hei + (y0 + y1)] * dC_da;
							///��Ϊ��padding������ֻ��һ���ֵ���
							if (x0 + x1 >= pad_w && x0 + x1 < wid - pad_w - 1 &&
								y0 + y1 >= pad_h && y0 + y1 < hei - pad_h - 1)
							{
								dC_dla[(x0 + x1 - pad_w) * in_shape[1] * in_dim + (y0 + y1 - pad_h) * in_dim + z0] +=
									w[wi] * dC_da;
							}
						}
					}
				}
				train_time++;
			}
		}
	}
	return dC_dla;
}
Array_F Conv2d::train_last(const Array_F& output, Loss loss, Regularizer reg)
{
	//����쳣
	if (output.size() != SizeOfShape(out_shape))throw_error(DnnLayerError::training_out_size);
	if (last_in.empty())throw_error(DnnLayerError::trainning_predict);

	Array_F dC_dla(in_size);//����һ���ƫ����

	//pad�������Ŀ��
	unsigned wid = in_shape[0] + 2 * pad_w;
	unsigned hei = in_shape[1] + 2 * pad_h;

	//���Ƶ�Ƕ��
	for (unsigned i = 0; i < kns; i++)
	{
		//x0, y0: �����(���Ͻ�)��λ��
		for (unsigned x0 = 0; x0 <= wid - kn_w; x0 += str_w)
		{
			for (unsigned y0 = 0; y0 <= hei - kn_h; y0 += str_h)
			{
				//��ǰz,a,output���±�
				unsigned temp = (x0 / str_w) * out_shape[1] * kns + (y0 / str_h) * kns + i;
				if (temp >= SizeOfShape(out_shape))continue;//Ӳ�˽��������
				//dC/da = da/dz * dC/da
				Dnn_F dC_da = dAct(z[temp]) * dLss(a[temp], output[temp], loss);
				b_g[i] += dC_da;

				//x1, y1: ���ڼ����λ��(����ھ����)
				for (unsigned x1 = 0; x1 < kn_w; x1++)
				{
					for (unsigned y1 = 0; y1 < kn_h; y1++)
					{
						//z0: �����ͨ����
						for (unsigned z0 = 0; z0 < in_dim; z0++)
						{
							//��ǰw���±�
							unsigned wi = i * kn_w * kn_h * in_dim + x1 * kn_h * in_dim + y1 * in_dim + z0;
							w_g[wi] += last_in[(x0 + x1) * hei * in_dim + (y0 + y1) * in_dim + z0] * dC_da;
							///��Ϊ��padding������ֻ��һ���ֵ���
							if (x0 + x1 >= pad_w && x0 + x1 < wid - pad_w - 1 &&
								y0 + y1 >= pad_h && y0 + y1 < hei - pad_h - 1)
							{
								dC_dla[(x0 + x1 - pad_w) * in_shape[1] * in_dim + (y0 + y1 - pad_h) * in_dim + z0] +=
									w[wi] * dC_da;
							}
						}
					}
				}
				train_time++;
			}
		}
	}
	return dC_dla;
}
void Conv2d::fit(Dnn_F lr, Optimizer opt)
{
	t++;//һ��Ҫ���ڿ�ͷ
	if (train_time != 0)
	{
		if (opt == Optimizer::bgd || opt == Optimizer::sgd)
		{
			for (unsigned i = 0; i < b.size(); i++)
			{
				//�ݶ�����
				if (b_g[i] > g_limit)b_g[i] = g_limit;
				else if (b_g[i] < -g_limit)b_g[i] = -g_limit;

				b[i] -= b_g[i] / train_time * lr;
			}
			for (unsigned i = 0; i < w.size(); i++)
			{
				//�ݶ�����
				if (w_g[i] > g_limit)w_g[i] = g_limit;
				else if (w_g[i] < -g_limit)w_g[i] = -g_limit;

				w[i] -= w_g[i] / train_time * lr;
			}
		}
		else if (opt == Optimizer::adam || opt == Optimizer::adam_as)
		{
			float m_hat, v_hat;
			for (unsigned i = 0; i < b.size(); i++)
			{
				//�ݶ�����
				if (b_g[i] > g_limit)b_g[i] = g_limit;
				else if (b_g[i] < -g_limit)b_g[i] = -g_limit;

				b_m[i] = adam_beta_1 * b_m[i] + (1 - adam_beta_1) * b_g[i];
				b_v[i] = adam_beta_2 * b_v[i] + (1 - adam_beta_2) * pow(b_g[i], 2);
				m_hat = b_m[i] / (1 - pow(adam_beta_1, t));
				v_hat = b_v[i] / (1 - pow(adam_beta_2, t));
				b[i] -= lr / (sqrt(v_hat) + adam_epsilon) * m_hat;
			}
			for (unsigned i = 0; i < w.size(); i++)
			{
				//�ݶ�����
				if (w_g[i] > g_limit)w_g[i] = g_limit;
				else if (w_g[i] < -g_limit)w_g[i] = -g_limit;

				w_m[i] = adam_beta_1 * w_m[i] + (1 - adam_beta_1) * w_g[i];
				w_v[i] = adam_beta_2 * w_v[i] + (1 - adam_beta_2) * pow(w_g[i], 2);
				m_hat = w_m[i] / (1 - pow(adam_beta_1, t));
				v_hat = w_v[i] / (1 - pow(adam_beta_2, t));
				w[i] -= lr / (sqrt(v_hat) + adam_epsilon) * m_hat;
			}
		}

		//��ղ���ʼ��
		w_g.clear();
		w_g.resize(w.size());
		b_g.clear();
		b_g.resize(b.size());
		train_time = 0;
	}
}

/*****************************************************************************
* Pool2d
* ��ά�ػ�(�²���)��
*****************************************************************************/
Pool2d::Pool2d(Shape input_shape, PoolType _pt,
	unsigned kernel_w, unsigned kernel_h,
	unsigned stride_w, unsigned stride_h)
{
	//Ԥ����һ��
	if (input_shape.size() == 2)input_shape.push_back(1);
	if (stride_w == -1 || stride_h == -1)
	{
		stride_w = kernel_w;
		stride_h = kernel_h;
	}
	//����쳣
	if (input_shape.size() != 3 || SizeOfShape(input_shape) == 0)throw_error(DnnLayerError::creating_shape);
	if (kernel_w == 0 || kernel_h == 0 ||
		stride_w == 0 || stride_h == 0 ||
		kernel_w > input_shape[0] || kernel_h > input_shape[1] ||
		stride_w > input_shape[0] || stride_h > input_shape[1])
	{
		throw_error(DnnLayerError::creating_invalid);
	}

	//��ʼ���ṹ����
	in_size = SizeOfShape(input_shape);
	in_dim = input_shape[2];
	pt = _pt;
	kn_w = kernel_w;
	kn_h = kernel_h;
	str_w = stride_w;
	str_h = stride_h;

	in_shape = input_shape;
	out_shape.resize(3);
	out_shape[0] = (in_shape[0] - kn_w) / str_w + 1;
	out_shape[1] = (in_shape[1] - kn_h) / str_h + 1;
	out_shape[2] = input_shape[2];
}
Pool2d::Info Pool2d::get_info()
{
	return
	{
		pt,
		kn_w, kn_h,
		str_w, str_h
	};
}
Array_F Pool2d::predict(const Array_F& input)
{
	//����쳣
	if (input.size() != in_size)throw_error(DnnLayerError::predicting_size);

	//��ղ���ʼ��
	a.clear();
	a.resize(SizeOfShape(out_shape));

	last_in = input;
	//���Ƶ�Ƕ��
	//x0, y0: �����(���Ͻ�)��λ��
	for (unsigned x0 = 0; x0 <= in_shape[0] - kn_w; x0 += str_w)
	{
		for (unsigned y0 = 0; y0 <= in_shape[1] - kn_h; y0 += str_h)
		{
			//z0: �����ͨ����
			for (unsigned z0 = 0; z0 < in_dim; z0++)
			{
				//��ǰa���±�
				unsigned temp = (x0 / str_w) * out_shape[1] * in_dim + (y0 / str_h) * in_dim + z0;
				if (temp >= SizeOfShape(out_shape))continue;//Ӳ�˽��������

				float max = input[x0 * in_shape[1] * in_dim + y0 * in_dim + z0];//���ֵ
				float sum = 0;//��
				//x1, y1: ���ڼ����λ��(����ھ����)
				for (unsigned x1 = 0; x1 < kn_w; x1++)
				{
					for (unsigned y1 = 0; y1 < kn_h; y1++)
					{
						//���ڷ����е�
						float now = input[(x0 + x1) * in_shape[1] * in_dim + (y0 + y1) * in_dim + z0];
						switch (pt)
						{
						case PoolType::maxpool:
							if (now > max)max = now;
							break;
						case PoolType::avgpool:
							sum += now;
							break;
						}
					}
				}

				switch (pt)
				{
				case PoolType::maxpool:
					a[temp] = max;
					break;
				case PoolType::avgpool:
					a[temp] = sum / kn_w / kn_h;
					break;
				}
			}
		}
	}
	return a;
}
Array_F Pool2d::train(const Array_F& dvt)
{
	//����쳣
	if (dvt.size() != SizeOfShape(out_shape))throw_error(DnnLayerError::training_dvt_size);
	if (last_in.empty())throw_error(DnnLayerError::trainning_predict);

	Array_F dC_dla(in_size);//����һ���ƫ����

	//���Ƶ�Ƕ��
	//x0, y0: �����(���Ͻ�)��λ��
	for (unsigned x0 = 0; x0 <= in_shape[0] - kn_w; x0 += str_w)
	{
		for (unsigned y0 = 0; y0 <= in_shape[1] - kn_h; y0 += str_h)
		{
			//z0: �����ͨ����
			for (unsigned z0 = 0; z0 < in_dim; z0++)
			{
				//��ǰa,dvt���±�
				unsigned temp = (x0 / str_w) * out_shape[1] * in_dim + (y0 / str_h) * in_dim + z0;
				if (temp >= SizeOfShape(out_shape))continue;//Ӳ�˽��������

				float max = last_in[x0 * in_shape[1] * in_dim + y0 * in_dim + z0];//���ֵ
				unsigned max_x = 0, max_y = 0;//���ֵ��λ��
				//x1, y1: ���ڼ����λ��(����ھ����)
				for (unsigned x1 = 0; x1 < kn_w; x1++)
				{
					for (unsigned y1 = 0; y1 < kn_h; y1++)
					{
						//���ڷ����е�
						float now = last_in[(x0 + x1) * in_shape[1] * in_dim + (y0 + y1) * in_dim + z0];
						switch (pt)
						{
						case PoolType::maxpool:
							if (now > max)max = now;
							max_x = x1;
							max_y = y1;
							break;
						case PoolType::avgpool:
							dC_dla[(x0 + x1) * in_shape[1] * in_dim + (y0 + y1) * in_dim + z0] =
								dvt[temp] / kn_w / kn_h;
							break;
						}
					}
				}
				if (pt == PoolType::maxpool)
				{
					for (unsigned x1 = 0; x1 < kn_w; x1++)
					{
						for (unsigned y1 = 0; y1 < kn_h; y1++)
						{
							if (x1 == max_x && y1 == max_y)
							{
								dC_dla[(x0 + x1) * in_shape[1] * in_dim + (y0 + y1) * in_dim + z0] = dvt[temp];
							}
							else
							{
								dC_dla[(x0 + x1) * in_shape[1] * in_dim + (y0 + y1) * in_dim + z0] = 0;
							}
						}
					}
				}
			}
		}
	}
	return dC_dla;
}
Array_F Pool2d::train_last(const Array_F& output, Loss loss)
{
	//����쳣
	if (output.size() != SizeOfShape(out_shape))throw_error(DnnLayerError::training_out_size);
	if (last_in.empty())throw_error(DnnLayerError::trainning_predict);

	Array_F dC_dla(in_size);//����һ���ƫ����

	//���Ƶ�Ƕ��
	//x0, y0: �����(���Ͻ�)��λ��
	for (unsigned x0 = 0; x0 <= in_shape[0] - kn_w; x0 += str_w)
	{
		for (unsigned y0 = 0; y0 <= in_shape[1] - kn_h; y0 += str_h)
		{
			//z0: �����ͨ����
			for (unsigned z0 = 0; z0 < in_dim; z0++)
			{
				//��ǰa,output���±�
				unsigned temp = (x0 / str_w) * out_shape[1] * in_dim + (y0 / str_h) * in_dim + z0;
				if (temp >= SizeOfShape(out_shape))continue;//Ӳ�˽��������

				//��ǰλ�õ�dvt
				float dvt = dLss(a[temp], output[temp], loss);
				float max = last_in[x0 * in_shape[1] * in_dim + y0 * in_dim + z0];//���ֵ
				unsigned max_x = 0, max_y = 0;//���ֵ��λ��
				//x1, y1: ���ڼ����λ��(����ھ����)
				for (unsigned x1 = 0; x1 < kn_w; x1++)
				{
					for (unsigned y1 = 0; y1 < kn_h; y1++)
					{
						//���ڷ����е�
						float now = last_in[(x0 + x1) * in_shape[1] * in_dim + (y0 + y1) * in_dim + z0];
						switch (pt)
						{
						case PoolType::maxpool:
							if (now > max)max = now;
							max_x = x1;
							max_y = y1;
							break;
						case PoolType::avgpool:
							dC_dla[(x0 + x1) * in_shape[1] * in_dim + (y0 + y1) * in_dim + z0] =
								dvt / kn_w / kn_h;
							break;
						}
					}
				}
				if (pt == PoolType::maxpool)
				{
					for (unsigned x1 = 0; x1 < kn_w; x1++)
					{
						for (unsigned y1 = 0; y1 < kn_h; y1++)
						{
							if (x1 == max_x && y1 == max_y)
							{
								dC_dla[(x0 + x1) * in_shape[1] * in_dim + (y0 + y1) * in_dim + z0] = dvt;
							}
							else
							{
								dC_dla[(x0 + x1) * in_shape[1] * in_dim + (y0 + y1) * in_dim + z0] = 0;
							}
						}
					}
				}
			}
		}
	}
	return dC_dla;
}

/*ģ��*/

/*****************************************************************************
* DnnModel
* ������ģ�� ����
*****************************************************************************/
void* DnnModel::get_layer(unsigned idx)
{
	if (idx >= layers.size())return nullptr;
	return layers[idx];
}
LayerType DnnModel::get_layer_type(unsigned idx)
{
	if (idx >= layerTypes.size())return LayerType::undefined;
	return layerTypes[idx];
}
void DnnModel::fit_init(Optimizer _opt, Dnn_F _lr, unsigned _batch_size, Loss _loss, Regularizer _reg)
{
	opt = _opt;
	loss = _loss;
	reg = _reg;
	lr = _lr;
	batch_size = _batch_size;
}

using namespace models;

/*****************************************************************************
* Sequential
* ˳��������ģ��
*****************************************************************************/
Sequential::~Sequential()
{
	clear();
}
Shape Sequential::get_input_shape()
{
	if (!layers.empty())
	{
		return ((DnnLayer*)layers.front())->get_input_shape();
	}
	else return {};
}
Shape Sequential::get_output_shape()
{
	if (!layers.empty())
	{
		return ((DnnLayer*)layers.back())->get_output_shape();
	}
	else return {};
}

bool Sequential::save(std::wstring filename)
{
	File f;
	bool res = f.open(filename, L"wb");
	if (!res)return false;

	//�ļ���Ϣ
	f.write("Data-Type: Dnn-Model\n");
	f.write("Model-Type: Sequential\n\n");

	//ģ�ͻ�����Ϣ
	Info mi;
	mi.layers = layers.size();
	mi.opt = opt;
	mi.batch_shape = batch_size;
	mi.lr = lr;
	mi.loss = loss;
	mi.reg = reg;
	f.write(&mi, sizeof(Info), 1);

	//���
	for (unsigned i = 0; i < mi.layers; i++)
	{
		//���ݲ����ͷֱ�洢
		LayerType type = layerTypes[i];
		f.write(&type, sizeof(LayerType), 1);
		//TODO: ����²�ʱ����µ� case ��ǩ
		switch (type)
		{
		case LayerType::input:
		{
			Input* p = (Input*)layers[i];
			f.write(p->get_output_shape());
			break;
		}
		case LayerType::trans:
		{
			Trans* p = (Trans*)layers[i];
			f.write(p->get_input_shape());
			f.write(p->get_output_shape());
			break;
		}
		case LayerType::dense:
		{
			Dense* p = (Dense*)layers[i];
			f.write(&p->act, sizeof(Activation), 1);
			auto info = p->get_info();
			f.write(&info, sizeof(Dense::Info), 1);
			//����
			f.write(p->w);
			f.write(p->b);
			break;
		}
		case LayerType::conv2d:
		{
			Conv2d* p = (Conv2d*)layers[i];
			f.write(&p->act, sizeof(Activation), 1);
			auto info = p->get_info();
			f.write(&info, sizeof(Conv2d::Info), 1);
			//����
			f.write(p->w);
			f.write(p->b);
			break;
		}
		case LayerType::pool2d:
		{
			Pool2d* p = (Pool2d*)layers[i];
			auto info = p->get_info();
			f.write(&info, sizeof(Pool2d::Info), 1);
			break;
		}
		}
	}
	f.close();
	return true;
}
bool Sequential::load(std::wstring filename)
{
	File f;
	bool res = f.open(filename, L"rb");
	if (!res)return false;

	//�ļ���Ϣ
	std::string buf;
	char ch;
	while (buf.find("\n\n") == std::string::npos && f.get_cur() < 128)
	{
		if (f.get_cur() == f.get_size())return false;
		f.read(&ch, sizeof(ch), sizeof(char), 1);
		buf += ch;
	}
	if (buf.find("Data-Type: Dnn-Model") == std::string::npos)return false;
	if (buf.find("Model-Type: Sequential") == std::string::npos)return false;

	//ģ�ͻ�����Ϣ
	Info mi;
	f.read(&mi, sizeof(mi), sizeof(Info), 1);

	//���
	clear();
	for (unsigned i = 0; i < mi.layers; i++)
	{
		//���ݲ����ͷֱ��ȡ
		LayerType type;
		f.read(&type, sizeof(type), sizeof(LayerType), 1);
		//TODO: ����²�ʱ����µ� case ��ǩ
		switch (type)
		{
		case LayerType::input:
		{
			Shape shape;
			f.read(shape);
			add(layers::Input(shape));
			break;
		}
		case LayerType::trans:
		{
			Shape in, out;
			f.read(in);
			f.read(out);
			add(layers::Trans(in, out));
			break;
		}
		case LayerType::dense:
		{
			Activation act;
			f.read(&act, sizeof(act), sizeof(Activation), 1);
			Dense::Info info;
			f.read(&info, sizeof(info), sizeof(Dense::Info), 1);
			add(layers::Dense(get_output_shape(), act, info.units));
			//����
			f.read(((Dense*)layers[i])->w);
			f.read(((Dense*)layers[i])->b);
			break;
		}
		case LayerType::conv2d:
		{
			Activation act;
			f.read(&act, sizeof(act), sizeof(Activation), 1);
			Conv2d::Info info;
			f.read(&info, sizeof(info), sizeof(Conv2d::Info), 1);
			add(layers::Conv2d(get_output_shape(), act, info.kns,
				info.kn_w, info.kn_h, info.pad_w, info.pad_h,
				info.str_w, info.str_h));
			//����
			f.read(((Conv2d*)layers[i])->w);
			f.read(((Conv2d*)layers[i])->b);
			break;
		}
		case LayerType::pool2d:
		{
			Pool2d::Info info;
			f.read(&info, sizeof(info), sizeof(Pool2d::Info), 1);
			add(layers::Pool2d(get_output_shape(), info.pt, info.kn_w, info.kn_h,
				info.str_w, info.str_h));
			break;
		}
		}
	}
	f.close();
	return true;
}

bool Sequential::add(const layers::Input& layer)
{
	if (!layers.empty())return false;

	Input* p = new Input;
	*p = layer;
	layers.push_back(p);
	layerTypes.push_back(LayerType::input);
	return true;
}
bool Sequential::add(const layers::Trans& layer)
{
	if (layers.empty())return false;

	Trans* p = new Trans;
	*p = layer;
	layers.push_back(p);
	layerTypes.push_back(LayerType::trans);
	return true;
}
bool Sequential::add(const layers::Dense& layer)
{
	if (layers.empty())return false;

	Dense* p = new Dense;
	*p = layer;
	layers.push_back(p);
	layerTypes.push_back(LayerType::dense);
	return true;
}
bool Sequential::add(const layers::Conv2d& layer)
{
	if (layers.empty())return false;

	Conv2d* p = new Conv2d;
	*p = layer;
	layers.push_back(p);
	layerTypes.push_back(LayerType::conv2d);
	return true;
}
bool Sequential::add(const layers::Pool2d& layer)
{
	if (layers.empty())return false;

	Pool2d* p = new Pool2d;
	*p = layer;
	layers.push_back(p);
	layerTypes.push_back(LayerType::pool2d);
	return true;
}

void Sequential::clear()
{
	for (void* p : layers)delete p;
	layers.clear();
}
Array_F Sequential::predict(const Array_F& input)
{
	//����쳣
	if (layers.empty())throw_error(DnnModelError::empty);
	if (input.size() != SizeOfShape(get_input_shape()))throw_error(DnnModelError::predicting_size);

	Input* p = (Input*)layers.front();
	p->set(input);
	Array_F output = p->predict();
	for (unsigned i = 1; i < layers.size(); i++)
	{
		//TODO: ����²�ʱ����µ� case ��ǩ
		switch (layerTypes[i])
		{
		case LayerType::trans:
			break;
		case LayerType::dense:
			output = ((Dense*)layers[i])->predict(output);
			break;
		case LayerType::conv2d:
			output = ((Conv2d*)layers[i])->predict(output);
			break;
		case LayerType::pool2d:
			output = ((Pool2d*)layers[i])->predict(output);
			break;
		}
	}
	return output;
}
Dnn_F Sequential::test(const std::vector<Array_F>& x_test, const std::vector<Array_F>& y_test)
{
	//����쳣
	if (layers.empty())throw_error(DnnModelError::empty);
	if (x_test.size() != y_test.size())throw_error(DnnModelError::fitting_size);

	Dnn_F res = 0;
	unsigned time = 0;
	for (unsigned i = 0; i < x_test.size(); i++)
	{
		//����쳣
		if (x_test[i].size() != SizeOfShape(get_input_shape()))throw_error(DnnModelError::fitting_in_size);
		if (y_test[i].size() != SizeOfShape(get_output_shape()))throw_error(DnnModelError::fitting_out_size);

		//����Ԥ��
		Array_F temp = predict(x_test[i]);
		for (unsigned j = 0; j < temp.size(); j++)
		{
			res += Lss(temp[j], y_test[i][j], loss) / x_test.size();
		}
	}
	return res;
}
Dnn_F Sequential::fit(const std::vector<Array_F>& x_train, const std::vector<Array_F>& y_train)
{
	//����쳣
	if (layers.empty())throw_error(DnnModelError::empty);
	if (x_train.size() != y_train.size())throw_error(DnnModelError::fitting_size);
	if (opt == Optimizer::undefined || loss == Loss::undefined || reg == Regularizer::undefined)
	{
		throw_error(DnnModelError::fitting_init);
	}

	//����
	Dnn_F res = test(x_train, y_train);
	
	//�Զ�ֹͣ
	if (opt == Optimizer::adam_as)
	{
		if (min_loss < 10 && res >= 1.1 * min_loss)
		{
			//ͣ��
			opt = Optimizer::sgd;
		}
	}

	unsigned time = 0;
	for (unsigned i = 0; i < x_train.size(); i++)
	{
		//����쳣
		if (x_train[i].size() != SizeOfShape(get_input_shape()))throw_error(DnnModelError::fitting_in_size);
		if (y_train[i].size() != SizeOfShape(get_output_shape()))throw_error(DnnModelError::fitting_out_size);

		//����������Ԥ��
		predict(x_train[i]);

		Array_F dvt;
		//ѵ�����һ��
		if (layers.size() > 1)
		{
			//TODO: ����²�ʱ����µ� case ��ǩ
			switch (layerTypes.back())
			{
			case LayerType::trans:
				break;
			case LayerType::dense:
				dvt = ((Dense*)layers.back())->train_last(y_train[i], loss, reg);
				break;
			case LayerType::conv2d:
				dvt = ((Conv2d*)layers.back())->train_last(y_train[i], loss, reg);
				break;
			case LayerType::pool2d:
				dvt = ((Pool2d*)layers.back())->train_last(y_train[i], loss);
				break;
			}
		}
		//ѵ��ʣ�µĲ�(��һ�㲻ѵ��)
		for (unsigned j = layers.size() - 2; j > 0; j--)
		{
			//TODO: ����²�ʱ����µ� case ��ǩ
			switch (layerTypes[j])
			{
			case LayerType::trans:
				break;
			case LayerType::dense:
				dvt = ((Dense*)layers[j])->train(dvt, reg);
				break;
			case LayerType::conv2d:
				dvt = ((Conv2d*)layers[j])->train(dvt, reg);
				break;
			case LayerType::pool2d:
				dvt = ((Pool2d*)layers[j])->train(dvt);
				break;
			}
		}

		time++;
		//ִ���ݶ��½�
		if (opt == Optimizer::sgd ||
			opt == Optimizer::bgd && time >= batch_size ||
			opt == Optimizer::adam ||
			opt == Optimizer::adam_as)
		{
			for (int j = 0; j < layers.size(); j++)
			{
				//TODO: ����²�ʱ����µ� case ��ǩ
				switch (layerTypes[j])
				{
				case LayerType::trans:
					break;
				case LayerType::dense:
					((Dense*)layers[j])->fit(lr, opt);
					break;
				case LayerType::conv2d:
					((Conv2d*)layers[j])->fit(lr, opt);
					break;
				}
			}
			time = 0;
		}
	}
	//ִ���ݶ��½�
	for (int j = 0; j < layers.size(); j++)
	{
		//TODO: ����²�ʱ����µ� case ��ǩ
		switch (layerTypes[j])
		{
		case LayerType::trans:
			break;
		case LayerType::dense:
			((Dense*)layers[j])->fit(lr, opt);
			break;
		case LayerType::conv2d:
			((Conv2d*)layers[j])->fit(lr, opt);
			break;
		}
	}
	if (res < min_loss)min_loss = res;
	return res;
}