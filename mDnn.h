#pragma once
/*****************************************************************************
* mDnn.h
* 神经网络
*
* 用法：
* - 大的框架和 Keras 差不多，可是又有一些细节差别
* - 第一层必须为 layers::Input
* 多维数组约定举例：
* - 一维 [xs]			a[x]				(0<=x<xs)
* - 二维 [xs][ys]		a[x*ys+y]			(0<=x<xs, 0<=y<=ys)
* - 三维 [xs][ys][zs]	a[x*ys*zs+y*zs+z]	(0<=x<xs, 0<=y<=ys, 0<=z<zs)
* - 以此类推...
*****************************************************************************/
typedef float Dnn_F;

#include <cmath>
#include <vector>
#include <stdexcept>//异常处理
//GPU 加速，冲冲冲！
#include <amp.h>
#include <amp_math.h>

#include "mFile.h"

namespace mlib
{
	typedef std::vector<Dnn_F> Array_F;
	typedef std::vector<unsigned> Shape;
	inline unsigned SizeOfShape(const Shape& shape)
	{
		if (shape.empty())return 0;
		unsigned res = 1;
		for (unsigned i : shape)res *= i;
		return res;
	}

#include "mDnn/AOLR.h"	//激活函数、优化器、损失函数、正则化方法

	//层类型
	enum class LayerType
	{
		undefined,
		input,	//输入层
		trans,	//转换层
		dense,	//全连接层
		conv2d,	//二维卷积层
		pool2d	//二维池化层
	};
	/*****************************************************************************
	* DnnLayer
	* 神经网络层 基类
	*****************************************************************************/
	class DnnLayer
	{
	protected:
		//输入大小
		unsigned in_size;
		//输入输出 Shape
		Shape in_shape;
		Shape out_shape;

		Array_F last_in;

		//激活函数
		Dnn_F Act(Dnn_F x);
		Dnn_F dAct(Dnn_F x);

		//用来抛异常的
		enum class DnnLayerError
		{
			creating_shape,
			creating_invalid,
			predicting_size,
			trainning_predict,
			training_dvt_size,
			training_out_size,
			input_setting,
			trans_creating,
			conv2d_padding
		};
		void throw_error(DnnLayerError err)
		{
			using namespace std;
			switch (err)
			{
			case DnnLayerError::creating_shape:
				throw invalid_argument("DnnLayer Creating: Size of input is invalid.");
				break;
			case DnnLayerError::creating_invalid:
				throw invalid_argument("DnnLayer Creating: Invalid params.");
				break;
			case DnnLayerError::predicting_size:
				throw invalid_argument("DnnLayer::predict(): Size of input is invalid.");
				break;
			case DnnLayerError::trainning_predict:
				throw logic_error("DnnLayer::train(): Run predict() first.");
				break;
			case DnnLayerError::training_out_size:
				throw invalid_argument("DnnLayer::train_last(): Size of output is invalid.");
				break;
			case DnnLayerError::training_dvt_size:
				throw invalid_argument("DnnLayer::train(): Size of derivative is invalid.");
				break;
			case DnnLayerError::input_setting:
				throw invalid_argument("Input::set(): Size of provided value is invalid.");
				break;
			case DnnLayerError::trans_creating:
				throw invalid_argument("Trans Creating: Size of input and that of output must be same.");
				break;
			case DnnLayerError::conv2d_padding:
				throw invalid_argument("Conv2d Creating: Width or height of kernel isn't odd. It's impossiple to make output size equals to input size.");
				break;
			default:
				break;
			}
		}

	public:
		Activation act;
		Array_F a;

		//获取输入输出的 Shape
		Shape get_input_shape() const;
		Shape get_output_shape() const;
	};

	//池化类型
	enum class PoolType
	{
		maxpool,	//最大池化
		meanpool	//均值池化
	};

#include"mDnn/layers.h" //神经网络层

	/*****************************************************************************
	* DnnModel
	* 神经网络模型 基类
	*****************************************************************************/
	class DnnModel
	{
	protected:
		std::vector<void*>		layers;
		std::vector<LayerType>	layerTypes;
		//用于 adam_as
		float min_loss = 1e10;
	public:
		//训练参数
		Optimizer opt = Optimizer::undefined;	//优化器
		unsigned batch_size = 0;				//每批大小
		Dnn_F lr = 0;							//学习率
		Loss loss = Loss::undefined;			//损失函数
		Regularizer reg = Regularizer::undefined;//正则化方法

		//获取层
		void* get_layer(unsigned idx);
		LayerType get_layer_type(unsigned idx);

		/// <summary>
		/// 初始化训练参数
		/// </summary>
		/// <param name="_opt">优化器</param>
		/// <param name="_lr">学习率</param>
		/// <param name="_batch_size">每批大小(用于BGD优化器)</param>
		/// <param name="_loss">损失函数</param>
		/// <param name="_reg">正则化方法</param>
		void fit_init(Optimizer _opt, Dnn_F _lr, unsigned _batch_size = -1, Loss _loss = Loss::mse, Regularizer _reg = Regularizer::none);

		//用来抛异常的
		enum class DnnModelError
		{
			empty,
			predicting_size,
			fitting_init,
			fitting_init_invalid,
			fitting_size,
			fitting_in_size,
			fitting_out_size
		};
		void throw_error(DnnModelError err)
		{
			using namespace std;
			switch (err)
			{
			case DnnModelError::empty:
				throw logic_error("DnnModel: The model is empty.");
				break;
			case DnnModelError::predicting_size:
				throw invalid_argument("DnnModel::predict(): Size of input is invalid.");
				break;
			case DnnModelError::fitting_init:
				throw logic_error("DnnModel::fit(): Haven't called fit_init() yet.");
				break;
			case DnnModelError::fitting_init_invalid:
				throw invalid_argument("DnnModel::fit_init(): Invalid params.");
				break;
			case DnnModelError::fitting_size:
				throw invalid_argument("DnnModel::fit(): Size of x_train and that of y_train must be same.");
				break;
			case DnnModelError::fitting_in_size:
				throw invalid_argument("DnnModel::fit(): Size of input is invalid.");
				break;
			case DnnModelError::fitting_out_size:
				throw invalid_argument("DnnModel::fit(): Size of output is invalid.");
				break;
			default:
				break;
			}
		}

		//清空模型
		virtual void clear() = 0;
		//预测(运行)
		virtual Array_F predict(const Array_F& input) = 0;
		//测试
		virtual Dnn_F test(const std::vector<Array_F>& x_test, const std::vector<Array_F>& y_test) = 0;
		//训练(必须先初始化)
		virtual Dnn_F fit(const std::vector<Array_F>& x_train, const std::vector<Array_F>& y_train) = 0;
	};
	namespace models
	{
		/*****************************************************************************
		* Sequential
		* 顺序神经网络模型
		*****************************************************************************/
		class Sequential :public DnnModel
		{
		public:
			struct Info
			{
				unsigned layers;		//层数
				//训练参数
				Optimizer opt;			//优化器
				unsigned batch_shape;	//每批大小
				Dnn_F lr;				//学习率
				Loss loss;				//损失函数
				Regularizer reg;		//正则化方法
			};
			~Sequential();

			//获取输入输出的 Shape
			Shape get_input_shape();
			Shape get_output_shape();

			//保存、加载模型(结构+参数)
			bool save(std::wstring filename);
			bool load(std::wstring filename);

			//添加新的神经网路层
			bool add(const layers::Input& layer);
			bool add(const layers::Trans& layer);
			bool add(const layers::Dense& layer);
			bool add(const layers::Conv2d& layer);
			bool add(const layers::Pool2d& layer);

			void clear();

			/// <summary>
			/// 预测(运行)
			/// </summary>
			/// <param name="input">模型的输入</param>
			/// <returns>模型的输出</returns>
			Array_F predict(const Array_F& input);
			
			/// <summary>
			/// 测试
			/// </summary>
			/// <param name="x_train">训练数据集.输入，[数据集大小,...]</param>
			/// <param name="y_train">训练数据集.期望输出，[数据集大小,...]</param>
			/// <returns>模型的损失函数值</returns>
			Dnn_F test(const std::vector<Array_F>& x_test, const std::vector<Array_F>& y_test);

			/// <summary>
			/// 训练
			/// </summary>
			/// <param name="x_train">训练数据集.输入，[数据集大小,...]</param>
			/// <param name="y_train">训练数据集.期望输出，[数据集大小,...]</param>
			/// <returns>模型(训练前)的损失函数值</returns>
			Dnn_F fit(const std::vector<Array_F>& x_train, const std::vector<Array_F>& y_train);
		};
	}
}