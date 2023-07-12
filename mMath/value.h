#pragma once
/*****************************************************************************
* Value
* ��ֵ��(��Ҫ�����Զ���)
*
*****************************************************************************/

namespace mlib
{
	class Value
	{
	private:
		//�����ϵ
		enum class Optimizer
		{
			null,				//�����㣬������
			add, sub, mlt, div,	//+ - * /
			pow, log,			//�˷�������
			dvt					//��

		};

		Optimizer opt;			//�����ϵ
		Math_F rv;			//��ʵֵ
		const Value* a, * b;//����������

		//��(���������ϵ,����һ����һ��)
		Math_F get_dvt(const Value& x) const
		{
			//���� 
			if (this == &x)return 1;
			//������ĳ���
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
		//����
		explicit Value(Math_F x = 0)
		{
			opt = Optimizer::null;
			rv = x;
			a = b = nullptr;
		}

		//������ֵ
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

		//�˷���a^b
		Value pow(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Optimizer::pow;
			return res;
		}
		//������log_b(a)
		Value log(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Optimizer::log;
			return res;
		}
		//��Ȼ������ln(a)
		Value ln() const
		{
			static Value e(M_E);
			return log(e);
		}

		//a��b�󵼣�da/db
		Value dvt(const Value& b) const
		{
			Value res;
			res.a = this;
			res.b = &b;
			res.opt = Optimizer::dvt;
			return res;
		}

		/*�� const ����*/

		//��ֵ�����ǽ��������ϵ
		Value& operator=(const Value& _b)
		{
			static Value zero(0);
			a = &_b;
			b = &zero;
			opt = Optimizer::add;
			return *this;
		}

		/*���¹��ܽ���opt==null*/

		//������ֵ
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

	static std::vector<Value> const_value_objects;//��¼���е� Value ��������
	//����
	inline Value vl(Math_F x)
	{
		//���ҿ��Ƿ��Ѿ�������
		for (size_t i = 0; i < const_value_objects.size(); i++)
		{
			if (const_value_objects[i].get() == x)return const_value_objects[i];
		}
		//�����µ�
		const_value_objects.push_back(Value(x));
		return const_value_objects.back();
	}
}