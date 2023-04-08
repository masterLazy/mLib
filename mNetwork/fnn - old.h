#pragma once
/*****************************************************************************
* fnn.h
* 全连接层
*
*****************************************************************************/

namespace fnn
{
	/*************************************************************************
	* Layer
	* 层
	*
	*************************************************************************/

	class Layer
	{
	private:
		int size;
		int fun;	//使用的激活函数

	public:
		Layer* parent;		//父层(上一层)
		vector<Math_F> z;	//没送进激活函数的
		vector<Math_F> a;	//激活值
		vector<Math_F> w;	//权重, w_i(本层)j(上层)=w[i*parent_size+j]
		vector<Math_F> b;	//偏置

		vector<Math_F> da;	//激活值 偏导值
		vector<Math_F> wC;	//权重 改变值
		vector<Math_F> bC;	//偏置 改变值

		//初始化
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

		//获取层大小(节点数)
		int Size()
		{
			return size;
		}

		//计算激活值
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

		//学习新的训练样本(反向传播)
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
		//应用训练样本(随机梯度下降)
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
	* 神经网络
	*
	*************************************************************************/

	void Train(vector<Layer>& layer, vector<Math_F>& output, int fun)
	{
#ifdef GPU
		using namespace concurrency;

		for (int i = layer.size() - 1; i > 0; i--)
		{
			array_view<Math_F, 1> temp(layer[i].Size());
			//偏置(计算temp)
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

			//权重
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

			//上一层(da)
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
		int times;		//训练次数
		Math_F times_h;	//训练次数(累计)

		//初始化(sizes: 各层的节点数, _fun: 激活函数)
		void Init(vector<int> size, int _fun = 0)
		{
			fun = _fun;
			layer.resize(size.size());
			for (int i = 0; i < size.size(); i++)
			{
				layer[i].Init(size[i], _fun, i > 0 ? &layer[i - 1] : nullptr);
			}
		}

		//获取输出
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

		//学习新的训练样本
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
		//应用训练样本
		bool EndTrain(Math_F learningRate)
		{
			bool res = layer[layer.size() - 1].EndTrain(times, learningRate);
			times = 0;
			return res;
		}

		//保存到文件
		bool Save(const char* filename)
		{
			FILE* f;
			fopen_s(&f, filename, "w");
			if (f == NULL)return false;

			//数据类型
			fprintf_s(f, "Data_type: Network\n");
			//使用的激活函数
			fprintf_s(f, "Activation_function: ");
			switch (fun)
			{
			case SIGMOID: fprintf_s(f, "Sigmoid\n"); break;
			case TANH: fprintf_s(f, "Tanh\n"); break;
			case RELU: fprintf_s(f, "ReLU\n"); break;
			case LRELU: fprintf_s(f, "LReLU\n"); break;
			}
			//层数
			fprintf_s(f, "Layers: %zu\n", layer.size());
			//训练次数
			fprintf_s(f, "Training_times: %g\n", times_h);

			//各个层
			for (int i = 0; i < layer.size(); i++)
			{
				fprintf_s(f, "\nLayer: %d\n", i);
				//节点数
				fprintf_s(f, "Nodes: %d\n", layer[i].Size());
				//激活值
				for (int j = 0; j < layer[i].Size(); j++)
				{
					fprintf_s(f, "a%d: %g\n", j, layer[i].a[j]);
				}
				//权重
				if (i > 0)
				{
					for (int j = 0; j < layer[i].Size(); j++)
					{
						for (int k = 0; k < layer[i - 1].Size(); k++)
						{
							fprintf_s(f, "w%d_%d: %g\n", j, k, layer[i].w[j * layer[i - 1].Size() + k]);
						}
					}
					//偏置
					for (int j = 0; j < layer[i].Size(); j++)
					{
						fprintf_s(f, "b%d: %g\n", j, layer[i].b[j]);
					}
				}
			}
			fclose(f);
			return true;
		}
		//从文件读取
		bool Load(const char* filename)
		{
			FILE* f;
			fopen_s(&f, filename, "r");
			if (f == NULL)return false;

			char temp[100];
			size_t size;

			//数据类型
			fscanf_s(f, "%s", temp, sizeof(temp));
			fscanf_s(f, "%s", temp, sizeof(temp));
			if (strcmp(temp, "Network") != 0)return false;
			//使用的激活函数
			fscanf_s(f, "%s", temp, sizeof(temp));
			fscanf_s(f, "%s", temp, sizeof(temp));

			if (strcmp(temp, "Sigmoid") == 0)fun = SIGMOID;
			else if (strcmp(temp, "Tanh") == 0)fun = TANH;
			else if (strcmp(temp, "ReLU") == 0)fun = RELU;
			else if (strcmp(temp, "LReLU") == 0)fun = LRELU;
			//层数
			fscanf_s(f, "%s %zu", temp, sizeof(temp), &size);
			layer.resize(size);
			//训练次数
			fscanf_s(f, "%s %g", temp, sizeof(temp), &times_h);
			//各个层
			for (int i = 0; i < layer.size(); i++)
			{
				fscanf_s(f, "%s %s", temp, sizeof(temp), temp, sizeof(temp));
				//节点数
				fscanf_s(f, "%s %d", temp, sizeof(temp), &size);
				layer[i].Init(size, fun, i > 0 ? &layer[i - 1] : nullptr);
				//激活值
				for (int j = 0; j < layer[i].Size(); j++)
				{
					fscanf_s(f, "%s", temp, sizeof(temp));
					fscanf_s(f, "%g", &layer[i].a[j]);
				}
				//权重
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
					//偏置
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