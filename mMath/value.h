#pragma once
/*****************************************************************************
* Value
* 求值链(主要用于自动求导)
*
*****************************************************************************/

namespace mlib
{
	class Value
	{
	private:
		//运算关系
		enum class Optimizer
		{
			null,				//无运算，即常量
			add, sub, mlt, div,	//+ - * /
			pow, log,			//乘方，对数
			dvt					//求导

		};

		Optimizer opt;			//运算关系
		Math_F rv;			//真实值
		const Value* a, * b;//两个运算数

		//求导(不是运算关系,调用一次求一次)
		Math_F get_dvt(const Value& x) const
		{
			//自身 
			if (this == &x)return 1;
			//非自身的常量
			else if (opt == Optimizer::null)return 0;

			Math_F av = a->get();
			Math_F bv = b->get();
			Math_F da = a->get_dvt(x);
			Math_F db = b->get_dvt(x);

			switch (opt)
			{
			case Optimizer::add:return da + db;
			case Optimizer::sub:return da - db;
				//[f(x)g(x)]'=f'(x)g(x)+f(x)g'(x)
			case Optimizer::mlt:return da * bv + av * db;
				//[f(x)/g(x)]'=[f'(x)g(x)-f(x)g'(x)]/[g(x)]^2
			case Optimizer::div:return (da * bv - av * db) / (bv * bv);
			case Optimizer::pow:
				if (av == 0)
				{
					return 0;
				}
				else
				{
					//[f(x)^g(x)]'=f(x)^g(x)*{g'(x)*ln[f(x)]+g(x)/f(x)*f'(x)}
					return ::pow(a->get(), bv) * (db * ::log(abs(av)) + bv / av * da);
				}
			}
		}

	public:
		//变量
		explicit Value(Math_F x = 0)
		{
			opt = Optimizer::null;
			rv = x;
			a = b = nullptr;
		}

		//计算数值
		Math_F get() const
		{
			switch (opt)
			{
			case Optimizer::null:return rv;
			case Optimizer::add:return a->get() + b->get();
			case Optimizer::sub:return a->get() - b->get();
			case Optimizer::mlt:return a->get() * b->get();
			case Optimizer::div:return a->get() / b->get();
			case Optimizer::pow:return ::pow(a->get(), b->get());
			case Optimizer::log:return ::log(a->get()) / ::log(b->get());
			case Optimizer::dvt:return a->get_dvt(*b);
			}
		}

		//赋值
		Math_F operator=(Math_F b)
		{
			set(b);
			return b;
		}

		//比较
		bool operator==(const Value& b) const
		{
			return get() == b.get();
		}
		bool operator!=(const Value& b) const
		{
			return get() != b.get();
		}
		bool operator<(const Value& b) const
		{
			return get() < b.get();
		}
		bool operator>(const Value& b) const
		{
			return get() > b.get();
		}
		bool operator<=(const Value& b) const
		{
			return get() <= b.get();
		}
		bool operator>=(const Value& b) const
		{
			return get() >= b.get();
		}

		/*建立运算关系*/

		//基本运算
		Value operator+(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Optimizer::add;
			return res;
		}
		Value operator-(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Optimizer::sub;
			return res;
		}
		Value operator*(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Optimizer::mlt;
			return res;
		}
		Value operator/(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Optimizer::div;
			return res;
		}

		Value operator-() const
		{
			Value res;
			static Value zero(0);
			res.a = &zero;
			res.b = this;
			res.opt = Optimizer::sub;
			return res;
		}

		//乘方，a^b
		Value pow(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Optimizer::pow;
			return res;
		}
		//对数，log_b(a)
		Value log(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Optimizer::log;
			return res;
		}
		//自然对数，ln(a)
		Value ln() const
		{
			static Value e(M_E);
			return log(e);
		}

		//a对b求导，da/db
		Value dvt(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Optimizer::dvt;
			return res;
		}

		/*非 const 运算*/

		//赋值，但是建立运算关系
		Value& operator=(const Value& _b)
		{
			static Value zero(0);
			a = &_b;
			b = &zero;
			opt = Optimizer::add;
			return *this;
		}

		/*以下功能仅限opt==null*/

		//设置数值
		bool set(Math_F x)
		{
			if (opt != Optimizer::null)return false;
			rv = x;
			return true;
		}

		bool operator+=(Math_F x)
		{
			if (opt != Optimizer::null)return false;
			rv += x;
			return true;
		}
		bool operator-=(Math_F x)
		{
			if (opt != Optimizer::null)return false;
			rv -= x;
			return true;
		}
		bool operator*=(Math_F x)
		{
			if (opt != Optimizer::null)return false;
			rv *= x;
			return true;
		}
		bool operator/=(Math_F x)
		{
			if (opt != Optimizer::null)return false;
			rv /= x;
			return true;
		}
	};

	//a^b
	inline Value pow(const Value& a, const Value& b)
	{
		return a.pow(b);
	}
	//log_b(a)
	inline Value log(const Value& a, const Value& b)
	{
		return a.log(b);
	}
	inline Value ln(const Value& a)
	{
		return a.ln();
	}

	static std::vector<Value> const_value_objects;//记录所有的 Value 常量对象
	//常量
	inline Value vl(Math_F x)
	{
		//找找看是否已经创建了
		for (size_t i = 0; i < const_value_objects.size(); i++)
		{
			if (const_value_objects[i].get() == x)return const_value_objects[i];
		}
		//创建新的
		const_value_objects.push_back(Value(x));
		return const_value_objects.back();
	}
}