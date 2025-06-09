#pragma once
/**
 * @file		node_var.hpp
 * @brief		�����ڵ�
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/expr.hpp"

namespace mlib {
	namespace expr {
		/**
		* @brief �����ط�װ
		*/
		class __VarPool {
			std::unordered_map<std::string, ExprNode*> pool;
		public:
			~__VarPool();
			/**
			* @brief					��ȡ��������Ӧ�ı����ڵ�ָ��
			* @param name				������
			* @param throw_if_not_exist	�����ڵ㲻����ʱ�Ƿ�Ҫ�׳��쳣
			* @exception NodeNotExist
			*/
			ExprNode* get(std::string name, bool throw_if_not_exist = false);

			/** @brief ĳ���ƶ�Ӧ�ı����ڵ��Ƿ��Ѵ��� */
			bool exist(std::string name) const {
				return pool.find(name) != pool.end();
			}
			/** @brief ��ձ����� */
			void clear() {
				pool.clear();
			}
		} var_pool;

		/**
		* @brief �����ڵ�
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
			/** @brief �Ƿ��ǽṹ�ڵ� */
			bool isStructNode() const override {
				return false;
			}
			/** @brief ���ر�����ֵ */
			Real getValue() const override {
				return value;
			}
			/** @brief תΪ�ַ��� */
			std::string toString() const override {
				return name;
			}

			/** @brief ���ñ�����ֵ */
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
		* @brief					��ȡ��������Ӧ�ı����ڵ�ָ��
		* @param name				������
		* @param throw_if_not_exist	�����ڵ㲻����ʱ�Ƿ�Ҫ�׳��쳣
		* @exception NodeNotExist
		*/
		VarNode* var(std::string name, bool throw_if_not_exist = false) {
			return (VarNode*)var_pool.get(name, throw_if_not_exist);
		}
	} // namespace expr
} // namespace mlib