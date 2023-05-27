#pragma once
/*****************************************************************************
* value
* ��ֵ��(��Ҫ�����Զ���)
*
*****************************************************************************/

namespace mlib
{
	class Value
	{
	private:
		//�����ϵ(null ��ʾ������, ������)
		enum class Opt
		{
			null, add, sub, mlt, div, pow, log, dvt

		};

		Opt opt;			//�����ϵ
		Math_F rv;			//��ʵֵ
		const Value* a, * b;//����������

		//��(���������ϵ,����һ����һ��)
		Math_F get_dvt(const Value& x) const
		{
			//���� 
			if (this == &x)return 1;
			//������ĳ���
			else if (opt == Opt::null)return 0;

			Math_F av = a->get();
			Math_F bv = b->get();
			Math_F da = a->get_dvt(x);
			Math_F db = b->get_dvt(x);

			switch (opt)
			{
			case Opt::add:return da + db;
			case Opt::sub:return da - db;
				//[f(x)g(x)]'=f'(x)g(x)+f(x)g'(x)
			case Opt::mlt:return da * bv + av * db;
				//[f(x)/g(x)]'=[f'(x)g(x)-f(x)g'(x)]/[g(x)]^2
			case Opt::div:return (da * bv - av * db) / (bv * bv);
			case Opt::pow:
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
		Value(Math_F x = 0)
		{
			opt = Opt::null;
			rv = x;
			a = b = nullptr;
		}

		//������ֵ
		Math_F get() const
		{
			switch (opt)
			{
			case Opt::null:return rv;
			case Opt::add:return a->get() + b->get();
			case Opt::sub:return a->get() - b->get();
			case Opt::mlt:return a->get() * b->get();
			case Opt::div:return a->get() / b->get();
			case Opt::pow:return ::pow(a->get(), b->get());
			case Opt::log:return ::log(a->get()) / ::log(b->get());
			case Opt::dvt:return a->get_dvt(*b);
			}
		}

		//��ֵ
		Math_F operator=(Math_F b)
		{
			set(b);
			return b;
		}

		//�Ƚ�
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

		/*���������ϵ*/

		//��������
		Value operator+(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Opt::add;
			return res;
		}
		Value operator-(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Opt::sub;
			return res;
		}
		Value operator*(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Opt::mlt;
			return res;
		}
		Value operator/(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Opt::div;
			return res;
		}
		Value operator-() const
		{
			Value res;
			static Value zero = 0;
			res.a = &zero;
			res.b = this;
			res.opt = Opt::sub;
			return res;
		}

		//�˷���a^b
		Value pow(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Opt::pow;
			return res;
		}
		//������log_b(a)
		Value log(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Opt::log;
			return res;
		}
		//��Ȼ������ln(a)
		Value ln() const
		{
			return log(M_E);
		}

		//a��b�󵼣�da/db
		Value dvt(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Opt::dvt;
			return res;
		}

		//��Ԫ
		Value friend operator+(Math_F a, const Value& b);
		Value friend operator-(Math_F a, const Value& b);
		Value friend operator*(Math_F a, const Value& b);
		Value friend operator/(Math_F a, const Value& b);

		/*���¹��ܽ���opt==null*/

		//������ֵ
		bool set(Math_F x)
		{
			if (opt != Opt::null)return false;

			rv = x;
			return true;
		}
	};

	//a^b
	Value pow(const Value& a, const Value& b)
	{
		return a.pow(b);
	}
	//log_b(a)
	Value log(const Value& a, const Value& b)
	{
		return a.log(b);
	}
	Value ln(const Value& a)
	{
		return a.ln();
	}

	std::vector<Value> const_value_objects;//��¼���е� Value ��������
	//Value ����
	Value vl(Math_F x)
	{
		//���ҿ��Ƿ��Ѿ�������
		for (size_t i = 0; i < const_value_objects.size(); i++)
		{
			if (const_value_objects[i].get() == x)return const_value_objects[i];
		}
		//�����µ�
		const_value_objects.push_back(x);
		return const_value_objects.back();
	}

	//��Ԫ
	Value operator+(Math_F a, const Value& b)
	{
		return b + a;
	}
	Value operator-(Math_F a, const Value& b)
	{
		return Value(a) - b;
	}
	Value operator*(Math_F a, const Value& b)
	{
		return b * a;
	}
	Value operator/(Math_F a, const Value& b)
	{
		return Value(a) / b;
	}
}