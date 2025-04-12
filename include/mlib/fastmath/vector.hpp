#pragma once
/**
 * @file		vector.hpp
 * @brief		并行加速向量抽象接口
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/fastmath.hpp"
namespace mlib {
	namespace fastmath {
		/**
		 * @brief			并行加速向量抽象接口
		 * @details			作为向量的基类，要求实现基本运算功能。使用了奇异递归模板模式(CRTP)
		 * @param Float		向量的浮点类型
		 * @param Derived	派生类自身
		 */
		template <typename Float, typename Derived> class Vector {
		public:
			/**
			 * @brief	获取向量大小
			 * @return	向量的元素个数
			 */
			virtual size_t size() const = 0;

			/**
			 * @brief		逐元素求相反数
			 * @param res	存放状态的指针
			 * @return		运算结果
			 */
			virtual Derived& nagate(Result* res = nullptr) = 0;

			/**
			 * @brief			逐元素相加
			 * @param another	要相加的向量
			 * @param res		存放状态的指针
			 * @return			运算结果
			 */
			virtual Derived& add(const Derived& another, Result* res = nullptr) = 0;
			/**
			 * @brief		逐元素与实数相加
			 * @param f		要相加的实数
			 * @param res	存放状态的指针
			 * @return		运算结果
			 */
			virtual Derived& add(Float f, Result* res = nullptr) = 0;

			/**
			 * @brief			逐元素相减
			 * @param another	要相减的向量
			 * @param res		存放状态的指针
			 * @return			运算结果
			 */
			virtual Derived& sub(const Derived& another, Result* res = nullptr) = 0;
			/**
			 * @brief		逐元素与实数相减
			 * @param f		要相减的实数
			 * @param res	存放状态的指针
			 * @return		运算结果
			 */
			virtual Derived& sub(Float f, Result* res = nullptr) = 0;

			/**
			 * @brief			元素相乘
			 * @param another	要相乘的向量
			 * @param res		存放状态的指针
			 * @return			运算结果
			 */
			virtual Derived& mul(const Derived& another, Result* res = nullptr) = 0;
			/**
			 * @brief		逐元素乘以实数
			 * @param f		要相乘的实数
			 * @param res	存放状态的指针
			 * @return		运算结果
			 */
			virtual Derived& mul(Float f, Result* res = nullptr) = 0;

			/**
			 * @brief			逐元素相除
			 * @param another	要相除的向量
			 * @param res		存放状态的指针
			 * @return			运算结果
			 */
			virtual Derived& div(const Derived& another, Result* res = nullptr) = 0;
			/**
			 * @brief		逐元素除以实数
			 * @param f		要相除的实数
			 * @param res	存放状态的指针
			 * @return		运算结果
			 */
			virtual Derived& div(Float f, Result* res = nullptr) = 0;

			/**
			 * @brief			逐元素求幂
			 * @param another	要作为指数的向量
			 * @param res		存放状态的指针
			 * @return			运算结果
			 */
			virtual Derived& pow(const Derived& another, Result* res = nullptr) = 0;
			/**
			 * @brief		以实数为指数逐元素求幂
			 * @param f		要作为指数的实数
			 * @param res	存放状态的指针
			 * @return		运算结果
			 */
			virtual Derived& pow(Float f, Result* res = nullptr) = 0;
		};
	} // namespace fastmath
} // namespace mlib