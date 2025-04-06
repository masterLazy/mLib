#pragma once
/*
* [抽象接口]
* 并行加速向量
*/
#include "mlib/fastmath.hpp"
namespace mlib {
	namespace fastmath {
		template <typename Float, typename Derived> class Vector {
		public:
			// 获取向量大小
			virtual size_t size() const = 0;
			// 逐元素: 取相反数
			virtual Derived& nagate(Result* res = nullptr) = 0;
			// 逐元素: 相加
			virtual Derived& add(const Derived& another, Result* res = nullptr) = 0;
			virtual Derived& add(Float f, Result* res = nullptr) = 0;
			// 逐元素: 相减
			virtual Derived& sub(const Derived& another, Result* res = nullptr) = 0;
			virtual Derived& sub(Float f, Result* res = nullptr) = 0;
			// 逐元素: 相乘
			virtual Derived& mul(const Derived& another, Result* res = nullptr) = 0;
			virtual Derived& mul(Float f, Result* res = nullptr) = 0;
			// 逐元素: 相除
			virtual Derived& div(const Derived& another, Result* res = nullptr) = 0;
			virtual Derived& div(Float f, Result* res = nullptr) = 0;
			// 逐元素: 求幂
			virtual Derived& pow(const Derived& another, Result* res = nullptr) = 0;
			virtual Derived& pow(Float f, Result* res = nullptr) = 0;
		};
	} // namespace fastmath
} // namespace mlib