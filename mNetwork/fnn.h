#pragma once
/*****************************************************************************
* fnn.h
* 全连接神经网络
*
*****************************************************************************/

namespace mlib
{
	/*****************************************************************************
	* FnnLayer
	* 全连接层
	*
	*****************************************************************************/

	class FnnLayer
	{
	private:
		size_t last_size;
	public:
		//训练次数
		size_t times = 0;
		//参数
		Vector z;
		Vector a;
		Matrix w;//(本层,上层)
		Vector b;
		int fun;
		//改变值
		Matrix wC;
		Vector bC;

		FnnLayer(const size_t size, const size_t last_layer_size, const int _fun)
		{
			last_size = last_layer_size;

			z.resize(size);
			a.resize(size);
			w.resize(size, last_size);
			b.resize(size);

			wC.resize(size, last_size);
			bC.resize(size);

			//随机化
			a.work(_Random);
			w.work(_Random);
			b.work(_Random);

			fun = _fun;
		}

		//获取大小
		size_t size()
		{
			return a.size();
		}

		//正向传播
		Vector work(const Vector& input)
		{
			z.fill(0);//清空z
			z = w * input;
			z += b;
			for (size_t i = 0; i < size(); i++)
			{
				a[i] = Act(z[i], fun);
			}
			return a;
		}

		//获取偏导
		Vector get_da(const Vector& correct)
		{
			Vector res(size());
			for (size_t i = 0; i < size(); i++)
			{
				res[i] = 2 * (a[i] - correct.get(i));
			}
			return res;
		}

		//反向传播（返回上一层各项的偏导）
		Vector learn(const Vector& input, const Vector& da)
		{
			Vector res(last_size);
			for (size_t i = 0; i < size(); i++)
			{
				Math_F temp = dAct(z[i], fun) * da.get(i);

				for (size_t j = 0; j < last_size; j++)
				{
					wC(i, j) -= input.get(j) * temp;
					res[j] += w(i, j) * temp;
				}
				bC[i] -= temp;
			}
			times++;
			return res;
		}

		//梯度下降
		//l_rate：学习率；l_max：梯度修剪，-1为无限制
		void end_learn(const Math_F l_rate, const Math_F l_max = -1)
		{
			if (l_max != -1)
			{
				wC.max_limit(l_max);
				bC.max_limit(l_max);
			}
			if (times != 0)
			{
				w += wC / times * l_rate;
				b += bC / times * l_rate;

				wC.fill(0);
				bC.fill(0);
				times = 0;
			}
		}
	};

	/*****************************************************************************
	* Fnn
	* 全连接神经网络
	*
	*****************************************************************************/

	class Fnn
	{
	private:
		Math_F h_times = 0;	//累计训练次数
		size_t times = 0;	//训练次数

	public:
		vector<FnnLayer> layer;

		//获取大小
		size_t size()
		{
			return layer.size();
		}
		//获取训练次数
		size_t get_times()
		{
			return times;
		}
		//获取累计训练次数
		size_t get_h_times()
		{
			return h_times;
		}

		//添加层
		void add_layer(size_t size, int fun)
		{
			if (layer.empty())
			{
				layer.push_back(FnnLayer(size, 0, fun));
			}
			else
			{
				layer.push_back(FnnLayer(size, layer[layer.size() - 1].size(), fun));
			}
		}

		//正向传播
		Vector work(const Vector& input)
		{
			if (layer.empty())return Vector();

			layer[0].a = input;
			for (size_t i = 1; i < size(); i++)
			{
				layer[i].work(layer[i - 1].a);
			}

			return layer.back().a;
		}
		Vector work(const vector<Math_F>& input)
		{
			return work((Vector)input);
		}

		//反向传播（返回代价）
		Math_F learn(const Vector& correct)
		{
			if (layer.size() < 2)return INFINITY;
			if (correct.size() != layer.back().size())return INFINITY;

			Vector da = layer.back().learn(layer[size() - 2].a, layer.back().get_da(correct));
			for (size_t i = size() - 2; i > 0; i--)
			{
				da = layer[i].learn(layer[i - 1].a, da);
			}
			h_times += 1;
			times++;

			Vector cost = correct - layer.back().a;
			return (cost * cost).sum();
		}
		Math_F learn(const vector<Math_F>& correct)
		{
			return learn((Vector)correct);
		}

		//梯度下降
		void end_learn(const Math_F l_rate, const Math_F l_max = -1)
		{
			for (size_t i = size() - 1; i > 0; i--)
			{
				layer[i].end_learn(l_rate, l_max);
			}
			times = 0;
		}

		//保存
		bool save(const char filename[])
		{
			File f;
			bool res = f.open(filename, WRITE);
			if (!res)return false;

			//层数、h_times、times
			size_t size = layer.size();
			f.write(&size, sizeof(size_t));
			f.write(&h_times, sizeof(Math_F));
			f.write(&times, sizeof(size_t));

			//每层节点数、激活函数
			size_t size2;
			for (size_t i = 0; i < size; i++)
			{
				size2 = layer[i].size();
				f.write(&size2, sizeof(size_t));
				f.write(&layer[i].fun, sizeof(int));
			}

			//每层
			for (size_t i = 0; i < size; i++)
			{
				//训练次数
				f.write(&layer[i].times, sizeof(size_t));
				//参数
				auto z = layer[i].z.get_arr_c();
				for (auto j : z)f.write(&j, sizeof(j));

				auto a = layer[i].a.get_arr_c();
				for (auto j : a)f.write(&j, sizeof(j));

				auto w = layer[i].w.get_arr_c();
				for (auto j : w)f.write(&j, sizeof(j));

				auto b = layer[i].b.get_arr_c();
				for (auto j : b)f.write(&j, sizeof(j));
				//改变值
				auto wC = layer[i].wC.get_arr_c();
				for (auto j : wC)f.write(&j, sizeof(j));

				auto bC = layer[i].bC.get_arr_c();
				for (auto j : bC)f.write(&j, sizeof(j));
			}

			f.close();
		}

		//加载
		bool load(const char filename[])
		{
			File f;
			bool res = f.open(filename, READ);
			if (!res)return false;

			//层数、h_times、times
			size_t size;
			f.read(&size, sizeof(size_t));
			f.read(&h_times, sizeof(Math_F));
			f.read(&times, sizeof(size_t));

			//每层节点数、激活函数
			size_t size2;
			int fun;
			layer.clear();
			for (size_t i = 0; i < size; i++)
			{
				f.read(&size2, sizeof(size_t));
				f.read(&fun, sizeof(int));
				add_layer(size2, fun);
			}

			//每层
			Math_F temp;
			for (size_t i = 0; i < size; i++)
			{
				//训练次数
				f.read(&layer[i].times, sizeof(size_t));
				//参数
				auto z = layer[i].z.get_arr();
				for (size_t j = 0; j < z->size();j++)
				{
					f.read(&temp, sizeof(Math_F));
					(*z)[j] = temp;
				}

				auto a = layer[i].a.get_arr();
				for (size_t j = 0; j < a->size(); j++)
				{
					f.read(&temp, sizeof(Math_F));
					(*a)[j] = temp;
				}

				auto w = layer[i].w.get_arr();
				for (size_t j = 0; j < w->size(); j++)
				{
					f.read(&temp, sizeof(Math_F));
					(*w)[j] = temp;
				}

				auto b = layer[i].b.get_arr();
				for (size_t j = 0; j < b->size(); j++)
				{
					f.read(&temp, sizeof(Math_F));
					(*b)[j] = temp;
				}
				//改变值
				auto wC = layer[i].wC.get_arr();
				for (size_t j = 0; j < wC->size(); j++)
				{
					f.read(&temp, sizeof(Math_F));
					(*wC)[j] = temp;
				}

				auto bC = layer[i].bC.get_arr();
				for (size_t j = 0; j < bC->size(); j++)
				{
					f.read(&temp, sizeof(Math_F));
					(*bC)[j] = temp;
				}
			}

			f.close();
		}
	};
}