#pragma once
/*****************************************************************************
* layers.h
* �������
* - w[this_i*input_size+input_i]
*****************************************************************************/

namespace layers
{
	/*****************************************************************************
	* Input
	* �����
	*****************************************************************************/
	class Input :public DnnLayer
	{
	public:
		Input() {}
		/// <param name="shape">����� Shape</param>
		Input(Shape shape);

		Array_F predict();

		//������Ԫ��ֵ
		void set(const Array_F& values);
	};

	/*****************************************************************************
	* Trans
	* ת���� (ת�� Shape)
	*****************************************************************************/
	class Trans :public DnnLayer
	{
	public:
		Trans() {}
		/// <param name="input_shape">��������� Shape</param>
		/// <param name="output_shape">����ת���ɵ� Shape</param>
		Trans(Shape input_shape, Shape output_shape);
	};

	/*****************************************************************************
	* Dense
	* ȫ�����������
	*****************************************************************************/
	class Dense :public DnnLayer
	{
	private:
		Array_F w_g;//Ȩ�ص��ݶ�(��)
		Array_F b_g;//ƫ�õ��ݶ�(��)
		//���� Adam �Ż���
		Array_F w_m;
		Array_F w_v;
		Array_F b_m;
		Array_F b_v;
		unsigned t;				//���� fit() �Ĵ���
		unsigned train_time = 0;//�ݶȺ͵�n

		unsigned units;
	public:
		//����
		Array_F w;	//weight��Ȩ��
		Array_F b;	//bias��ƫ��
		Array_F z;

		Dense() {}
		/// <param name="input_shape">��������� Shape (����Ϊһά)</param>
		/// <param name="_units">��Ԫ����</param>
		/// <param name="activation">�����</param>
		Dense(Shape input_shape, mlib::Activation activation, unsigned _units);

		//��ȡ�ṹ��Ϣ
		struct Info
		{
			unsigned units;	//��Ԫ��
		};
		Info get_info();

		/// <summary>
		/// Ԥ�� (����)
		/// </summary>
		/// <param name="input">��������</param>
		/// <returns>�������</returns>
		Array_F predict(const Array_F& input);

		/// <summary>ѵ�� (�����ݶ�)</summary>
		/// <param name="dvt">���㼤��ֵ����ʧ������ƫ����ֵ</param>
		/// <param name="reg">���򻯷���</param>
		/// <returns>�ϲ㼤��ֵ����ʧ������ƫ����ֵ</returns>
		Array_F train(const Array_F& dvt, Regularizer reg = Regularizer::none);

		/// <summary>ѵ�� (�����ݶ�) (���������һ��ʱʹ��)</summary>
		/// <param name="output">������������</param>
		/// <param name="loss">��ʧ����</param>
		/// <param name="reg">���򻯷���</param>
		/// <returns>�ϲ㼤��ֵ����ʧ������ƫ����ֵ</returns>
		Array_F train_last(const Array_F& output, Loss loss, Regularizer reg = Regularizer::none);

		/// <summary>
		/// ѵ�� (Ӧ���ݶ��½�)
		/// </summary>
		/// <param name="lr">ѧϰ��</param>
		/// <param name="opt">�Ż���</param>
		void fit(Dnn_F lr, Optimizer opt);
	};

	/*****************************************************************************
	* Conv2d
	* ��ά����������
	*****************************************************************************/
	class Conv2d :public DnnLayer
	{
	private:
		Array_F w_g;//Ȩ�ص��ݶ�(��)
		Array_F b_g;//ƫ�õ��ݶ�(��)
		//���� Adam �Ż���
		Array_F w_m;
		Array_F w_v;
		Array_F b_m;
		Array_F b_v;
		unsigned t;				//���� fit() �Ĵ���
		unsigned train_time = 0;//�ݶȺ͵�n

		unsigned in_dim;
		unsigned kns, kn_w, kn_h;
		unsigned pad_w, pad_h;
		unsigned str_w, str_h;
	public:
		//����
		Array_F w;	//weight��Ȩ��
		Array_F b;	//bias��ƫ��
		Array_F z;

		Conv2d() {}
		/// <param name="input_shape">��������� Shape (��ά/��ά��[w][h][ͨ�����])</param>
		/// <param name="kernels">���������</param>
		/// <param name="kernel_w">����˵Ŀ�</param>
		/// <param name="kernel_h">����˵ĸ�</param>
		/// <param name="padding_w">���Ŀ�� (-1 ��ʾ�Զ�, ʹ��������������һ��)</param>
		/// <param name="padding_h">���ĸ߶� (-1 ��ʾ�Զ�, ʹ������߶�������һ��)</param>
		/// <param name="stride_w">������򲽳�</param>
		/// <param name="stride_h">������򲽳�</param>
		/// <param name="activation">�����</param>
		Conv2d(Shape input_shape,
			mlib::Activation activation,
			unsigned kernels,
			unsigned kernel_w, unsigned kernel_h,
			unsigned padding_w = 0, unsigned padding_h = 0,
			unsigned stride_w = 1, unsigned stride_h = 1);

		//��ȡ�ṹ��Ϣ
		struct Info
		{
			unsigned kns;			//����˸���
			unsigned kn_w, kn_h;	//����˿��
			unsigned pad_w, pad_h;	//�����
			unsigned str_w, str_h;	//������ݲ���
		};
		Info get_info();

		/// <summary>
		/// Ԥ�� (����)
		/// </summary>
		/// <param name="input">��������</param>
		/// <returns>������� (��ά��[w][h][ͨ�����])</returns>
		Array_F predict(const Array_F& input);

		/// <summary>ѵ�� (�����ݶ�)</summary>
		/// <param name="dvt">���㼤��ֵ����ʧ������ƫ����ֵ</param>
		/// <param name="reg">���򻯷���</param>
		/// <returns>�ϲ㼤��ֵ����ʧ������ƫ����ֵ</returns>
		Array_F train(const Array_F& dvt, Regularizer reg = Regularizer::none);

		/// <summary>ѵ�� (�����ݶ�) (���������һ��ʱʹ��)</summary>
		/// <param name="output">������������</param>
		/// <param name="loss">��ʧ����</param>
		/// <param name="reg">���򻯷���</param>
		/// <returns>�ϲ㼤��ֵ����ʧ������ƫ����ֵ</returns>
		Array_F train_last(const Array_F& output, Loss loss, Regularizer reg = Regularizer::none);

		/// <summary>
		/// ѵ�� (Ӧ���ݶ��½�)
		/// </summary>
		/// <param name="lr">ѧϰ��</param>
		/// <param name="opt">�Ż���</param>
		void fit(Dnn_F lr, Optimizer opt);
	};

	/*****************************************************************************
	* Pool2d
	* ��ά�ػ�(�²���)��
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
		/// <param name="input_shape">��������� Shape (��ά/��ά��[w][h][ͨ�����])</param>
		/// <param name="_pt">�ػ�����</param>
		/// <param name="kernel_w">�ػ��˵Ŀ�</param>
		/// <param name="kernel_h">�ػ��˵ĸ�</param>
		/// <param name="stride_w">�ػ����򲽳� (-1 ��ʾ�Զ�, ��ػ��˵Ŀ���ͬ)</param>
		/// <param name="stride_h">�ػ����򲽳� (-1 ��ʾ�Զ�, ��ػ��˵ĸ���ͬ)</param>
		Pool2d(Shape input_shape, PoolType _pt,
			unsigned kernel_w, unsigned kernel_h,
			unsigned stride_w = -1, unsigned stride_h = -1);

		//��ȡ�ṹ��Ϣ
		struct Info
		{
			PoolType pt;			//�ػ�����
			unsigned kn_w, kn_h;	//�ػ��˿��
			unsigned str_w, str_h;	//�ػ����ݲ���
		};
		Info get_info();

		/// <summary>
		/// Ԥ�� (����)
		/// </summary>
		/// <param name="input">��������</param>
		/// <returns>������� (��ά��[w][h][ͨ�����])</returns>
		Array_F predict(const Array_F& input);

		/// <summary>ѵ�� (�����ݶ�)</summary>
		/// <param name="input">��������</param>
		/// <param name="dvt">���㼤��ֵ����ʧ������ƫ����ֵ</param>
		/// <returns>�ϲ㼤��ֵ����ʧ������ƫ����ֵ</returns>
		Array_F train(const Array_F& dvt);

		/// <summary>ѵ�� (�����ݶ�) (���������һ��ʱʹ��)</summary>
		/// <param name="input">��������</param>
		/// <param name="output">������������</param>
		/// <param name="loss">��ʧ����</param>
		/// <returns>�ϲ㼤��ֵ����ʧ������ƫ����ֵ</returns>
		Array_F train_last(const Array_F& output, Loss loss);
	};
}