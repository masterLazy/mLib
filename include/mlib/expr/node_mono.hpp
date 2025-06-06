#pragma once
/**
 * @file		node_mono.hpp
 * @brief		����ʽ�ڵ�
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/expr.hpp"

namespace mlib {
	namespace expr {
		/**
		* @brief ����ʽ�ڵ�
		*/
		class MonoNode : public ExprNode {
			Real coe = 1;					// ϵ��
			std::vector<ExprNode*> child;	// ����(����������)
			std::vector<ExprNode*> exp;		// �����ָ��
		public:
			MonoNode() {
				this->node_type = NodeType::mono;
			}
			/** @brief �Ƿ��ǽṹ�ڵ� */
			bool isStructNode() const override {
				return true;
			}
			/** @brief ��������ֵ */
			Real getValue() const override {
				Real sum = coe;
				for (size_t i = 0; i < child.size(); i++) {
					sum *= pow(child[i]->getValue(), exp[i]->getValue());
				}
				return sum;
			}
			/** @brief תΪ�ַ��� */
			std::string toString() const override {
				std::vector<std::string> str;
				for (size_t i = 0; i < child.size(); i++) {
					if (exp[i]->getType() == NodeType::consta and exp[i]->getValue() == 1) {
						str.push_back(child[i]->toString());
					} else {
						str.push_back(child[i]->toString() + "^" + exp[i]->toString());
					}
				}
				sort(str.begin(), str.end());
				std::string res = coe == 1 ? "" : to_string(coe);
				for (size_t i = 0; i < child.size(); i++) {
					res += str[i];
				}
				return res;
			}

			/**
			* @brief	�������
			* @return	���������
			*/
			MonoNode& addItem(ExprNode* item, ExprNode* exp_of_item) {
				if (item->getType() == NodeType::consta and exp_of_item->getType() == NodeType::consta) {
					coe *= pow(item->getValue(), exp_of_item->getValue());
				} else {
					child.push_back(item);
					exp.push_back(exp_of_item);
				}
				return *this;
			}
		};
	} // namespace expr
} // namespace mlib