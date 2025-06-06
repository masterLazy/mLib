#pragma once
/**
 * @file		node_var.hpp
 * @brief		变量节点
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/expr.hpp"

namespace mlib {
	namespace expr {
		/**
		* @brief 变量池封装
		*/
		class __VarPool {
			std::unordered_map<std::string, ExprNode*> pool;
		public:
			~__VarPool();
			/**
			* @brief					获取变量名对应的变量节点指针
			* @param name				变量名
			* @param throw_if_not_exist	变量节点不存在时是否要抛出异常
			* @exception NodeNotExist
			*/
			ExprNode* get(std::string name, bool throw_if_not_exist = false);

			/** @brief 某名称对应的变量节点是否已存在 */
			bool exist(std::string name) const {
				return pool.find(name) != pool.end();
			}
			/** @brief 清空变量池 */
			void clear() {
				pool.clear();
			}
		} var_pool;

		/**
		* @brief 变量节点
		* @exception NodeAlreadyExist
		*/
		class VarNode : public ExprNode {
			std::string name;
			Real value;
		public:
			/**
			* @exception NodeAlreadyExist
			*/
			VarNode(std::string name, Real initial_value = 0) {
				if (var_pool.exist(name)) {
					throw NodeAlreadyExist;
				}
				this->node_type = NodeType::var;
				this->name = name;
				this->value = initial_value;
			}
			/** @brief 是否是结构节点 */
			bool isStructNode() const override {
				return false;
			}
			/** @brief 返回变量的值 */
			Real getValue() const override {
				return value;
			}
			/** @brief 转为字符串 */
			std::string toString() const override {
				return name;
			}

			/** @brief 设置变量的值 */
			void setValue(Real value_to_set) {
				value = value_to_set;
			}
		};

		__VarPool::~__VarPool() {
			for (auto i = pool.begin(); i != pool.end(); i++) {
				delete i->second;
			}
		}
		ExprNode* __VarPool::get(std::string name, bool throw_if_not_exist) {
			if (exist(name)) {
				return pool[name];
			} else {
				if (throw_if_not_exist) {
					throw NodeNotExist;
				}
				auto p = new VarNode(name);
				pool[name] = p;
				return p;
			}
		}

		/**
		* @brief					获取变量名对应的变量节点指针
		* @param name				变量名
		* @param throw_if_not_exist	变量节点不存在时是否要抛出异常
		* @exception NodeNotExist
		*/
		VarNode* var(std::string name, bool throw_if_not_exist = false) {
			return (VarNode*)var_pool.get(name, throw_if_not_exist);
		}
	} // namespace expr
} // namespace mlib