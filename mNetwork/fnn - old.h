#pragma once
/*****************************************************************************
* fnn.h
* ȫ���Ӳ�
*
*****************************************************************************/

namespace fnn
{
	/*************************************************************************
	* Layer
	* ��
	*
	*************************************************************************/

	class Layer
	{
	private:
		int size;
		int fun;	//ʹ�õļ����

	public:
		Layer* parent;		//����(��һ��)
		vector<Math_F> z;	//û�ͽ��������
		vector<Math_F> a;	//����ֵ
		vector<Math_F> w;	//Ȩ��, w_i(����)j(�ϲ�)=w[i*parent_size+j]
		vector<Math_F> b;	//ƫ��

		vector<Math_F> da;	//����ֵ ƫ��ֵ
		vector<Math_F> wC;	//Ȩ�� �ı�ֵ
		vector<Math_F> bC;	//ƫ�� �ı�ֵ

		//��ʼ��
		void Init(const int _size, int _fun = 0, Layer* const _parent = nullptr)
		{
			fun = _fun;
			size = _size;
			parent = _parent;

			z.resize(size);

			a.resize(size);
			da.resize(size);
			if (parent != nullptr)
			{
				w.resize(size * parent->size);
				wC.resize(size * parent->size);

				b.resize(size);
				bC.resize(size);
			}

			for (int i = 0; i < w.size(); i++)
			{
				w[i] = 1;//for TEST!!
				wC[i] = 0;
			}
			for (int i = 0; i < b.size(); i++)
			{
				b[i] = 1;//for TEST!!
				bC[i] = 0;
			}
		}

		//��ȡ���С(�ڵ���)
		int Size()
		{
			return size;
		}

		//���㼤��ֵ
		bool Work()
		{
			if (parent == nullptr)
			{
				return false;
			}
			for (int i = 0; i < size; i++)
			{
				z[i] = 0;
				for (int j = 0; j < parent->size; j++)
				{
					z[i] += parent->a[j] * w[i * parent->size + j];
				}
				z[i] += b[i];
				a[i] = Act(z[i], fun);
			}
			return true;
		}

		//ѧϰ�µ�ѵ������(���򴫲�)
		void NewTrain(vector<Math_F> _y)
		{
			if (parent == nullptr)return;
			if (_y.size() != size)return;

			for (int i = 0; i < size; i++)
			{
				Math_F temp = dAct(z[i], fun) * 2 * (a[i] - _y[i]);
				//cout << a[i] + aC[i] << endl;
				//cin.get();
				for (int j = 0; j < parent->size; j++)
				{
					wC[i * parent->size + j] -= parent->a[j] * temp;
					parent->da[j] += w[i * parent->size + j] * temp;
				}
				bC[i] -= temp;
			}
			da.clear();
			da.resize(size);
			parent->NewTrain();
		}
		void NewTrain()
		{
			if (parent == nullptr)return;

			for (int i = 0; i < size; i++)
			{
				Math_F temp = dAct(z[i], fun) * da[i];
				//cout << a[i] + aC[i] << endl;
				//cin.get();
				for (int j = 0; j < parent->size; j++)
				{
					wC[i * parent->size + j] -= parent->a[j] * temp;
					parent->da[j] += w[i * parent->size + j] * temp;
				}
				bC[i] -= temp;
			}
			da.clear();
			da.resize(size);
			parent->NewTrain();
		}
		//Ӧ��ѵ������(����ݶ��½�)
		bool EndTrain(int times, Math_F learningRate)
		{
			if (parent == nullptr)return true;
			if (times == 0)return false;

			a.clear();
			da.clear();
			a.resize(size);
			da.resize(size);
			for (int i = 0; i < w.size(); i++)
			{
				w[i] += wC[i] / times * learningRate;
				wC[i] = 0;
			}
			for (int i = 0; i < b.size(); i++)
			{
				b[i] += bC[i] / times * learningRate;
				bC[i] = 0;
			}
			return parent->EndTrain(times, learningRate);
		}
	};

	/*************************************************************************
	* Network
	* ������
	*
	*************************************************************************/

	void Train(vector<Layer>& layer, vector<Math_F>& output, int fun)
	{
#ifdef GPU
		using namespace concurrency;

		for (int i = layer.size() - 1; i > 0; i--)
		{
			array_view<Math_F, 1> temp(layer[i].Size());
			//ƫ��(����temp)
			array_view<Math_F, 1> gpu_z(layer[i].Size(), layer[i].z);
			array_view<Math_F, 1> gpu_a(layer[i].Size(), layer[i].a);
			bool last = i == layer.size() - 1;
			array_view<Math_F, 1> gpu_da_y(layer[i].Size(), last ? output : layer[i].da);
			array_view<Math_F, 1> gpu_bC(layer[i].Size(), layer[i].bC);

			parallel_for_each(
				gpu_a.extent,
				[=](index<1> idx) restrict(amp)
				{
					if (last)
					{
						temp[idx] = dAct(gpu_z[idx], fun) * 2 * (gpu_a[idx] - gpu_da_y[idx]);
					}
					else
					{
						temp[idx] = dAct(gpu_z[idx], fun) * 2 * gpu_da_y[idx];
					}
					gpu_bC[idx] -= temp[idx];
				}
			);
			gpu_bC.synchronize();

			//Ȩ��
			array_view<Math_F, 1> gpu_pa(layer[i - 1].Size(), layer[i - 1].a);
			array_view<Math_F, 2> gpu_wC(layer[i].Size(), layer[i - 1].Size(), layer[i].wC);

			parallel_for_each(
				gpu_wC.extent,
				[=](index<2> idx) restrict(amp)
				{
					gpu_wC[idx] -= gpu_pa[idx[1]] * temp[idx[0]];
				}
			);
			gpu_wC.synchronize();

			//��һ��(da)
			array_view<Math_F, 2> gpu_w(layer[i].Size(), layer[i - 1].Size(), layer[i].w);
			array_view<Math_F, 1> gpu_parent_a(layer[i - 1].Size(), layer[i - 1].a);

			parallel_for_each(
				gpu_w.extent,
				[=](index<2> idx) restrict(amp)
				{
					gpu_da_y[idx[1]] += gpu_w[idx] * temp[idx[0]];
				}
			);
		}
#endif
	}

	class Network
	{
	private:
		int fun;

	public:
		vector<Layer> layer;
		int times;		//ѵ������
		Math_F times_h;	//ѵ������(�ۼ�)

		//��ʼ��(sizes: ����Ľڵ���, _fun: �����)
		void Init(vector<int> size, int _fun = 0)
		{
			fun = _fun;
			layer.resize(size.size());
			for (int i = 0; i < size.size(); i++)
			{
				layer[i].Init(size[i], _fun, i > 0 ? &layer[i - 1] : nullptr);
			}
		}

		//��ȡ���
		bool Work(vector<Math_F> input, vector<Math_F>* output = nullptr)
		{
			if (input.size() != layer[0].Size())
			{
				return false;
			}
			layer[0].a = input;
			for (int i = 0; i < layer.size(); i++)
			{
				layer[i].Work();
			}

			if (output != nullptr)
			{
				output->resize(layer[layer.size() - 1].Size());
				for (int i = 0; i < layer[layer.size() - 1].Size(); i++)
				{
					(*output)[i] = layer[layer.size() - 1].a[i];
				}
			}
			return true;
		}

		//ѧϰ�µ�ѵ������
		bool NewTrain(vector<Math_F> output)
		{
#ifndef GPU
			if (output.size() != layer[layer.size() - 1].Size())
			{
				return false;
			}
			layer[layer.size() - 1].NewTrain(output);
#else
			Train(layer, output, fun);
#endif
			times++;
			times_h++;
			return true;
		}
		//Ӧ��ѵ������
		bool EndTrain(Math_F learningRate)
		{
			bool res = layer[layer.size() - 1].EndTrain(times, learningRate);
			times = 0;
			return res;
		}

		//���浽�ļ�
		bool Save(const char* filename)
		{
			FILE* f;
			fopen_s(&f, filename, "w");
			if (f == NULL)return false;

			//��������
			fprintf_s(f, "Data_type: Network\n");
			//ʹ�õļ����
			fprintf_s(f, "Activation_function: ");
			switch (fun)
			{
			case SIGMOID: fprintf_s(f, "Sigmoid\n"); break;
			case TANH: fprintf_s(f, "Tanh\n"); break;
			case RELU: fprintf_s(f, "ReLU\n"); break;
			case LRELU: fprintf_s(f, "LReLU\n"); break;
			}
			//����
			fprintf_s(f, "Layers: %zu\n", layer.size());
			//ѵ������
			fprintf_s(f, "Training_times: %g\n", times_h);

			//������
			for (int i = 0; i < layer.size(); i++)
			{
				fprintf_s(f, "\nLayer: %d\n", i);
				//�ڵ���
				fprintf_s(f, "Nodes: %d\n", layer[i].Size());
				//����ֵ
				for (int j = 0; j < layer[i].Size(); j++)
				{
					fprintf_s(f, "a%d: %g\n", j, layer[i].a[j]);
				}
				//Ȩ��
				if (i > 0)
				{
					for (int j = 0; j < layer[i].Size(); j++)
					{
						for (int k = 0; k < layer[i - 1].Size(); k++)
						{
							fprintf_s(f, "w%d_%d: %g\n", j, k, layer[i].w[j * layer[i - 1].Size() + k]);
						}
					}
					//ƫ��
					for (int j = 0; j < layer[i].Size(); j++)
					{
						fprintf_s(f, "b%d: %g\n", j, layer[i].b[j]);
					}
				}
			}
			fclose(f);
			return true;
		}
		//���ļ���ȡ
		bool Load(const char* filename)
		{
			FILE* f;
			fopen_s(&f, filename, "r");
			if (f == NULL)return false;

			char temp[100];
			size_t size;

			//��������
			fscanf_s(f, "%s", temp, sizeof(temp));
			fscanf_s(f, "%s", temp, sizeof(temp));
			if (strcmp(temp, "Network") != 0)return false;
			//ʹ�õļ����
			fscanf_s(f, "%s", temp, sizeof(temp));
			fscanf_s(f, "%s", temp, sizeof(temp));

			if (strcmp(temp, "Sigmoid") == 0)fun = SIGMOID;
			else if (strcmp(temp, "Tanh") == 0)fun = TANH;
			else if (strcmp(temp, "ReLU") == 0)fun = RELU;
			else if (strcmp(temp, "LReLU") == 0)fun = LRELU;
			//����
			fscanf_s(f, "%s %zu", temp, sizeof(temp), &size);
			layer.resize(size);
			//ѵ������
			fscanf_s(f, "%s %g", temp, sizeof(temp), &times_h);
			//������
			for (int i = 0; i < layer.size(); i++)
			{
				fscanf_s(f, "%s %s", temp, sizeof(temp), temp, sizeof(temp));
				//�ڵ���
				fscanf_s(f, "%s %d", temp, sizeof(temp), &size);
				layer[i].Init(size, fun, i > 0 ? &layer[i - 1] : nullptr);
				//����ֵ
				for (int j = 0; j < layer[i].Size(); j++)
				{
					fscanf_s(f, "%s", temp, sizeof(temp));
					fscanf_s(f, "%g", &layer[i].a[j]);
				}
				//Ȩ��
				if (i > 0)
				{
					for (int j = 0; j < layer[i].Size(); j++)
					{
						for (int k = 0; k < layer[i - 1].Size(); k++)
						{
							fscanf_s(f, "%s", temp, sizeof(temp));
							fscanf_s(f, "%g", &layer[i].w[j * layer[i - 1].Size() + k]);
						}
					}
					//ƫ��
					for (int j = 0; j < layer[i].Size(); j++)
					{
						fscanf_s(f, "%s", temp, sizeof(temp));
						fscanf_s(f, "%g", &layer[i].b[j]);
					}
				}
			}
			fclose(f);
			return true;
		}
	};
}