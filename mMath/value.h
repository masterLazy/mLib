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
		enum class Operation
		{
			null,				//无运算，即原始量
			add, sub, mlt, div,	//+ - * /
			pow, log,			//乘方，对数
			dvt					//求导

		};

		Operation opt;		//运算关系
		Math_F rv;			//真实值
		const Value* a, * b;//两个运算数

		//求导(不是运算关系,调用一次求一次)
		Math_F get_dvt(const Value& x) const;

	public:
		//创建变量
		explicit Value(Math_F x = 0);

		//计算数值
		Math_F get() const;

		//赋值
		Math_F operator=(Math_F b);

		//比较
		bool operator==(const Value& b) const;
		bool operator!=(const Value& b) const;
		bool operator<(const Value& b) const;
		bool operator>(const Value& b) const;
		bool operator<=(const Value& b) const;
		bool operator>=(const Value& b) const;

		/*建立运算关系*/

		//基本运算
		Value operator+(const Value& b) const;
		Value operator-(const Value& b) const;
		Value operator*(const Value& b) const;
		Value operator/(const Value& b) const;

		Value operator-() const;

		//乘方，a^b
		Value pow(const Value& b) const;
		//对数，log_b(a)
		Value log(const Value& b) const;
		//自然对数，ln(a)
		Value ln() const;

		//a对b求导，da/db
		Value dvt(const Value& b) const;

		/*非 const 运算*/

		//赋值，但是建立运算关系
		Value& operator=(const Value& _b);

		/*以下功能仅限opt==null*/

		//设置数值
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

	static std::vector<Value> const_value_objects;//记录所有的 Value 常量对象
	//常量
	inline Value cvl(Math_F x);
}