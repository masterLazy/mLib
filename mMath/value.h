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
		enum class Operation
		{
			null,				//�����㣬��ԭʼ��
			add, sub, mlt, div,	//+ - * /
			pow, log,			//�˷�������
			dvt					//��

		};

		Operation opt;		//�����ϵ
		Math_F rv;			//��ʵֵ
		const Value* a, * b;//����������

		//��(���������ϵ,����һ����һ��)
		Math_F get_dvt(const Value& x) const;

	public:
		//��������
		explicit Value(Math_F x = 0);

		//������ֵ
		Math_F get() const;

		//��ֵ
		Math_F operator=(Math_F b);

		//�Ƚ�
		bool operator==(const Value& b) const;
		bool operator!=(const Value& b) const;
		bool operator<(const Value& b) const;
		bool operator>(const Value& b) const;
		bool operator<=(const Value& b) const;
		bool operator>=(const Value& b) const;

		/*���������ϵ*/

		//��������
		Value operator+(const Value& b) const;
		Value operator-(const Value& b) const;
		Value operator*(const Value& b) const;
		Value operator/(const Value& b) const;

		Value operator-() const;

		//�˷���a^b
		Value pow(const Value& b) const;
		//������log_b(a)
		Value log(const Value& b) const;
		//��Ȼ������ln(a)
		Value ln() const;

		//a��b�󵼣�da/db
		Value dvt(const Value& b) const;

		/*�� const ����*/

		//��ֵ�����ǽ��������ϵ
		Value& operator=(const Value& _b);

		/*���¹��ܽ���opt==null*/

		//������ֵ
		bool set(Math_F x);

		bool operator+=(Math_F x);
		bool operator-=(Math_F x);
		bool operator*=(Math_F x);
		bool operator/=(Math_F x);
	};

	//a^b
	inline Value pow(const Value& a, const Value& b);
	//log_b(a)
	inline Value log(const Value& a, const Value& b);
	inline Value ln(const Value& a);

	static std::vector<Value> const_value_objects;//��¼���е� Value ��������
	//����
	inline Value cvl(Math_F x);
}