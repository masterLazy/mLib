#pragma once
/*****************************************************************************
* layers.h
* 神经网络层
* - w[this_i*input_size+input_i]
*****************************************************************************/

namespace layers
{
	/*****************************************************************************
	* Input
	* 输入层
	*****************************************************************************/
	class Input :public DnnLayer
	{
	public:
		Input() {}
		/// <param name="shape">本层的 Shape</param>
		Input(Shape shape);

		Array_F predict();

		//设置神经元的值
		void set(const Array_F& values);
	};

	/*****************************************************************************
	* Trans
	* 转换层 (转换 Shape)
	*****************************************************************************/
	class Trans :public DnnLayer
	{
	public:
		Trans() {}
		/// <param name="input_shape">本层输入的 Shape</param>
		/// <param name="output_shape">期望转换成的 Shape</param>
		Trans(Shape input_shape, Shape output_shape);
	};

	/*****************************************************************************
	* Dense
	* 全连接神经网络层
	*****************************************************************************/
	class Dense :public DnnLayer
	{
	private:
		Array_F w_g;//权重的梯度(和)
		Array_F b_g;//偏置的梯度(和)
		//用于 Adam 优化器
		Array_F w_m;
		Array_F w_v;
		Array_F b_m;
		Array_F b_v;
		unsigned t;				//调用 fit() 的次数
		unsigned train_time = 0;//梯度和的n

		unsigned units;
	public:
		//参数
		Array_F w;	//weight，权重
		Array_F b;	//bias，偏置
		Array_F z;

		Dense() {}
		/// <param name="input_shape">本层输入的 Shape (必须为一维)</param>
		/// <param name="_units">神经元数量</param>
		/// <param name="activation">激活函数</param>
		Dense(Shape input_shape, mlib::Activation activation, unsigned _units);

		//获取结构信息
		struct Info
		{
			unsigned units;	//神经元数
		};
		Info get_info();

		/// <summary>
		/// 预测 (运行)
		/// </summary>
		/// <param name="input">本层输入</param>
		/// <returns>本层输出</returns>
		Array_F predict(const Array_F& input);

		/// <summary>训练 (计算梯度)</summary>
		/// <param name="dvt">本层激活值对损失函数的偏导数值</param>
		/// <param name="reg">正则化方法</param>
		/// <returns>上层激活值对损失函数的偏导数值</returns>
		Array_F train(const Array_F& dvt, Regularizer reg = Regularizer::none);

		/// <summary>训练 (计算梯度) (本层是最后一层时使用)</summary>
		/// <param name="output">本层的期望输出</param>
		/// <param name="loss">损失函数</param>
		/// <param name="reg">正则化方法</param>
		/// <returns>上层激活值对损失函数的偏导数值</returns>
		Array_F train_last(const Array_F& output, Loss loss, Regularizer reg = Regularizer::none);

		/// <summary>
		/// 训练 (应用梯度下降)
		/// </summary>
		/// <param name="lr">学习率</param>
		/// <param name="opt">优化器</param>
		void fit(Dnn_F lr, Optimizer opt);
	};

	/*****************************************************************************
	* Conv2d
	* 二维卷积神经网络层
	*****************************************************************************/
	class Conv2d :public DnnLayer
	{
	private:
		Array_F w_g;//权重的梯度(和)
		Array_F b_g;//偏置的梯度(和)
		//用于 Adam 优化器
		Array_F w_m;
		Array_F w_v;
		Array_F b_m;
		Array_F b_v;
		unsigned t;				//调用 fit() 的次数
		unsigned train_time = 0;//梯度和的n

		unsigned in_dim;
		unsigned kns, kn_w, kn_h;
		unsigned pad_w, pad_h;
		unsigned str_w, str_h;
	public:
		//参数
		Array_F w;	//weight，权重
		Array_F b;	//bias，偏置
		Array_F z;

		Conv2d() {}
		/// <param name="input_shape">本层输入的 Shape (二维/三维，[w][h][通道编号])</param>
		/// <param name="kernels">卷积核数量</param>
		/// <param name="kernel_w">卷积核的宽</param>
		/// <param name="kernel_h">卷积核的高</param>
		/// <param name="padding_w">填充的宽度 (-1 表示自动, 使得输出宽度与输入一致)</param>
		/// <param name="padding_h">填充的高度 (-1 表示自动, 使得输出高度与输入一致)</param>
		/// <param name="stride_w">卷积横向步长</param>
		/// <param name="stride_h">卷积纵向步长</param>
		/// <param name="activation">激活函数</param>
		Conv2d(Shape input_shape,
			mlib::Activation activation,
			unsigned kernels,
			unsigned kernel_w, unsigned kernel_h,
			unsigned padding_w = 0, unsigned padding_h = 0,
			unsigned stride_w = 1, unsigned stride_h = 1);

		//获取结构信息
		struct Info
		{
			unsigned kns;			//卷积核个数
			unsigned kn_w, kn_h;	//卷积核宽高
			unsigned pad_w, pad_h;	//填充宽高
			unsigned str_w, str_h;	//卷积横纵步长
		};
		Info get_info();

		/// <summary>
		/// 预测 (运行)
		/// </summary>
		/// <param name="input">本层输入</param>
		/// <returns>本层输出 (三维，[w][h][通道编号])</returns>
		Array_F predict(const Array_F& input);

		/// <summary>训练 (计算梯度)</summary>
		/// <param name="dvt">本层激活值对损失函数的偏导数值</param>
		/// <param name="reg">正则化方法</param>
		/// <returns>上层激活值对损失函数的偏导数值</returns>
		Array_F train(const Array_F& dvt, Regularizer reg = Regularizer::none);

		/// <summary>训练 (计算梯度) (本层是最后一层时使用)</summary>
		/// <param name="output">本层的期望输出</param>
		/// <param name="loss">损失函数</param>
		/// <param name="reg">正则化方法</param>
		/// <returns>上层激活值对损失函数的偏导数值</returns>
		Array_F train_last(const Array_F& output, Loss loss, Regularizer reg = Regularizer::none);

		/// <summary>
		/// 训练 (应用梯度下降)
		/// </summary>
		/// <param name="lr">学习率</param>
		/// <param name="opt">优化器</param>
		void fit(Dnn_F lr, Optimizer opt);
	};

	/*****************************************************************************
	* Pool2d
	* 二维池化(下采样)层
	*****************************************************************************/
	class Pool2d :public DnnLayer
	{
	private:
		unsigned in_dim;
		PoolType pt;
		unsigned kn_w, kn_h;
		unsigned str_w, str_h;
	public:
		Pool2d() {}
		/// <param name="input_shape">本层输入的 Shape (二维/三维，[w][h][通道编号])</param>
		/// <param name="_pt">池化方法</param>
		/// <param name="kernel_w">池化核的宽</param>
		/// <param name="kernel_h">池化核的高</param>
		/// <param name="stride_w">池化横向步长 (-1 表示自动, 与池化核的宽相同)</param>
		/// <param name="stride_h">池化纵向步长 (-1 表示自动, 与池化核的高相同)</param>
		Pool2d(Shape input_shape, PoolType _pt,
			unsigned kernel_w, unsigned kernel_h,
			unsigned stride_w = -1, unsigned stride_h = -1);

		//获取结构信息
		struct Info
		{
			PoolType pt;			//池化方法
			unsigned kn_w, kn_h;	//池化核宽高
			unsigned str_w, str_h;	//池化横纵步长
		};
		Info get_info();

		/// <summary>
		/// 预测 (运行)
		/// </summary>
		/// <param name="input">本层输入</param>
		/// <returns>本层输出 (三维，[w][h][通道编号])</returns>
		Array_F predict(const Array_F& input);

		/// <summary>训练 (计算梯度)</summary>
		/// <param name="input">本层输入</param>
		/// <param name="dvt">本层激活值对损失函数的偏导数值</param>
		/// <returns>上层激活值对损失函数的偏导数值</returns>
		Array_F train(const Array_F& dvt);

		/// <summary>训练 (计算梯度) (本层是最后一层时使用)</summary>
		/// <param name="input">本层输入</param>
		/// <param name="output">本层的期望输出</param>
		/// <param name="loss">损失函数</param>
		/// <returns>上层激活值对损失函数的偏导数值</returns>
		Array_F train_last(const Array_F& output, Loss loss);
	};
}