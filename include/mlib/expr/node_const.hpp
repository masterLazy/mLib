#pragma once
/**
 * @file		node_const.hpp
 * @brief		常数节点
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/expr.hpp"

namespace mlib {
	namespace expr {
		/**
		* @brief 常数池封装
		*/
		class __ConstPool {
			std::unordered_map<Real, ExprNode*> pool;
		public:
			__ConstPool();
			~__ConstPool();
			/**
			* @brief					获取实值对应的常数节点指针
			* @param value				实值
			* @param throw_if_not_exist	常数节点不存在时是否要抛出异常
			* @exception NodeNotExist
			*/
			ExprNode* get(Real value, bool throw_if_not_exist = false);

			/** @brief 某实值对应的常数节点是否已存在 */
			bool exist(Real value) const {
				return pool.find(value) != pool.end();
			}
			/** @brief 清空常数池 */
			void clear() {
				pool.clear();
			}
		} const_pool;

		/**
		* @brief 常数节点
		* @exception NodeAlreadyExist
		*/
		class ConstNode : public ExprNode {
			Real value;
		public:
			/**
			* @exception NodeAlreadyExist
			*/
			ConstNode(Real value) {
				if (const_pool.exist(value)) {
					throw NodeAlreadyExist;
				}
				this->node_type = NodeType::consta;
				this->value = value;
			}
			/** @brief 是否是结构节点 */
			bool isStructNode() const override {
				return false;
			}
			/** @brief 返回实值 */
			Real getValue() const override {
				return value;
			}
			/** @brief 转为字符串 */
			std::string toString() const override {
				return to_string(value);
			}
		};


		__ConstPool::__ConstPool() {
			// 加入常见的数字
			for (int i = -10; i <= 10; i++) {
				pool[i] = new ConstNode(i);
			}
		}
		__ConstPool::~__ConstPool() {
			for (auto i = pool.begin(); i != pool.end(); i++) {
				delete i->second;
			}
		}
		ExprNode* __ConstPool::get(Real value, bool throw_if_not_exist) {
			if (exist(value)) {
				return pool[value];
			} else {
				if (throw_if_not_exist) {
					throw NodeNotExist;
				}
				auto p = new ConstNode(value);
				pool[value] = p;
				return p;
			}
		}

		/**
		* @brief					获取实值对应的常数节点指针
		* @param value				实值
		* @param throw_if_not_exist	常数节点不存在时是否要抛出异常
		* @exception NodeNotExist
		*/
		ConstNode* consta(Real value, bool throw_if_not_exist = false) {
			return (ConstNode*)const_pool.get(value, throw_if_not_exist);
		}
	} // namespace expr
} // namespace mlib