#pragma once
/**
 * @file		node_const.hpp
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
		class __ConstPool {
			std::unordered_map<Real, ExprNode*> pool;
		public:
			__ConstPool();
			~__ConstPool();
			/**
			* @brief					��ȡʵֵ��Ӧ�ĳ����ڵ�ָ��
			* @param value				ʵֵ
			* @param throw_if_not_exist	�����ڵ㲻����ʱ�Ƿ�Ҫ�׳��쳣
			* @exception NodeNotExist
			*/
			ExprNode* get(Real value, bool throw_if_not_exist = false);

			/** @brief ĳʵֵ��Ӧ�ĳ����ڵ��Ƿ��Ѵ��� */
			bool exist(Real value) const {
				return pool.find(value) != pool.end();
			}
			/** @brief ��ճ����� */
			void clear() {
				pool.clear();
			}
		} const_pool;

		/**
		* @brief �����ڵ�
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
			/** @brief �Ƿ��ǽṹ�ڵ� */
			bool isStructNode() const override {
				return false;
			}
			/** @brief ����ʵֵ */
			Real getValue() const override {
				return value;
			}
			/** @brief תΪ�ַ��� */
			std::string toString() const override {
				return to_string(value);
			}
		};


		__ConstPool::__ConstPool() {
			// ���볣��������
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
		* @brief					��ȡʵֵ��Ӧ�ĳ����ڵ�ָ��
		* @param value				ʵֵ
		* @param throw_if_not_exist	�����ڵ㲻����ʱ�Ƿ�Ҫ�׳��쳣
		* @exception NodeNotExist
		*/
		ConstNode* consta(Real value, bool throw_if_not_exist = false) {
			return (ConstNode*)const_pool.get(value, throw_if_not_exist);
		}
	} // namespace expr
} // namespace mlib