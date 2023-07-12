#pragma once
/*****************************************************************************
* mDnn.h
* ������
*
* �÷���
* - ��Ŀ�ܺ� Keras ��࣬��������һЩϸ�ڲ��
* - ��һ�����Ϊ layers::Input
* ��ά����Լ��������
* - һά [xs]			a[x]				(0<=x<xs)
* - ��ά [xs][ys]		a[x*ys+y]			(0<=x<xs, 0<=y<=ys)
* - ��ά [xs][ys][zs]	a[x*ys*zs+y*zs+z]	(0<=x<xs, 0<=y<=ys, 0<=z<zs)
* - �Դ�����...
*****************************************************************************/
typedef float Dnn_F;

#include <cmath>
#include <vector>
#include <stdexcept>//�쳣����
//GPU ���٣����壡
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

#include "mDnn/AOLR.h"	//��������Ż�������ʧ���������򻯷���

	//������
	enum class LayerType
	{
		undefined,
		input,	//�����
		trans,	//ת����
		dense,	//ȫ���Ӳ�
		conv2d,	//��ά�����
		pool2d	//��ά�ػ���
	};
	/*****************************************************************************
	* DnnLayer
	* ������� ����
	*****************************************************************************/
	class DnnLayer
	{
	protected:
		//�����С
		unsigned in_size;
		//������� Shape
		Shape in_shape;
		Shape out_shape;

		Array_F last_in;

		//�����
		Dnn_F Act(Dnn_F x);
		Dnn_F dAct(Dnn_F x);

		//�������쳣��
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

		//��ȡ��������� Shape
		Shape get_input_shape() const;
		Shape get_output_shape() const;
	};

	//�ػ�����
	enum class PoolType
	{
		maxpool,	//���ػ�
		meanpool	//��ֵ�ػ�
	};

#include"mDnn/layers.h" //�������

	/*****************************************************************************
	* DnnModel
	* ������ģ�� ����
	*****************************************************************************/
	class DnnModel
	{
	protected:
		std::vector<void*>		layers;
		std::vector<LayerType>	layerTypes;
		//���� adam_as
		float min_loss = 1e10;
	public:
		//ѵ������
		Optimizer opt = Optimizer::undefined;	//�Ż���
		unsigned batch_size = 0;				//ÿ����С
		Dnn_F lr = 0;							//ѧϰ��
		Loss loss = Loss::undefined;			//��ʧ����
		Regularizer reg = Regularizer::undefined;//���򻯷���

		//��ȡ��
		void* get_layer(unsigned idx);
		LayerType get_layer_type(unsigned idx);

		/// <summary>
		/// ��ʼ��ѵ������
		/// </summary>
		/// <param name="_opt">�Ż���</param>
		/// <param name="_lr">ѧϰ��</param>
		/// <param name="_batch_size">ÿ����С(����BGD�Ż���)</param>
		/// <param name="_loss">��ʧ����</param>
		/// <param name="_reg">���򻯷���</param>
		void fit_init(Optimizer _opt, Dnn_F _lr, unsigned _batch_size = -1, Loss _loss = Loss::mse, Regularizer _reg = Regularizer::none);

		//�������쳣��
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

		//���ģ��
		virtual void clear() = 0;
		//Ԥ��(����)
		virtual Array_F predict(const Array_F& input) = 0;
		//����
		virtual Dnn_F test(const std::vector<Array_F>& x_test, const std::vector<Array_F>& y_test) = 0;
		//ѵ��(�����ȳ�ʼ��)
		virtual Dnn_F fit(const std::vector<Array_F>& x_train, const std::vector<Array_F>& y_train) = 0;
	};
	namespace models
	{
		/*****************************************************************************
		* Sequential
		* ˳��������ģ��
		*****************************************************************************/
		class Sequential :public DnnModel
		{
		public:
			struct Info
			{
				unsigned layers;		//����
				//ѵ������
				Optimizer opt;			//�Ż���
				unsigned batch_shape;	//ÿ����С
				Dnn_F lr;				//ѧϰ��
				Loss loss;				//��ʧ����
				Regularizer reg;		//���򻯷���
			};
			~Sequential();

			//��ȡ��������� Shape
			Shape get_input_shape();
			Shape get_output_shape();

			//���桢����ģ��(�ṹ+����)
			bool save(std::wstring filename);
			bool load(std::wstring filename);

			//����µ�����·��
			bool add(const layers::Input& layer);
			bool add(const layers::Trans& layer);
			bool add(const layers::Dense& layer);
			bool add(const layers::Conv2d& layer);
			bool add(const layers::Pool2d& layer);

			void clear();

			/// <summary>
			/// Ԥ��(����)
			/// </summary>
			/// <param name="input">ģ�͵�����</param>
			/// <returns>ģ�͵����</returns>
			Array_F predict(const Array_F& input);
			
			/// <summary>
			/// ����
			/// </summary>
			/// <param name="x_train">ѵ�����ݼ�.���룬[���ݼ���С,...]</param>
			/// <param name="y_train">ѵ�����ݼ�.���������[���ݼ���С,...]</param>
			/// <returns>ģ�͵���ʧ����ֵ</returns>
			Dnn_F test(const std::vector<Array_F>& x_test, const std::vector<Array_F>& y_test);

			/// <summary>
			/// ѵ��
			/// </summary>
			/// <param name="x_train">ѵ�����ݼ�.���룬[���ݼ���С,...]</param>
			/// <param name="y_train">ѵ�����ݼ�.���������[���ݼ���С,...]</param>
			/// <returns>ģ��(ѵ��ǰ)����ʧ����ֵ</returns>
			Dnn_F fit(const std::vector<Array_F>& x_train, const std::vector<Array_F>& y_train);
		};
	}
}