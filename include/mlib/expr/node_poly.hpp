#pragma once
/**
 * @file		node_poly.hpp
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
		class PolyNode : public ExprNode {
			Real const_sum = 0;				// ���г���
			std::vector<ExprNode*> child;	// ����(����������)
		public:
			PolyNode() {
				this->node_type = NodeType::poly;
			}
			/** @brief �Ƿ��ǽṹ�ڵ� */
			bool isStructNode() const override {
				return true;
			}
			/** @brief ��������ֵ */
			Real getValue() const override {
				Real sum = const_sum;
				for (auto p : child) {
					sum += p->getValue();
				}
				return sum;
			}
			/** @brief תΪ�ַ��� */
			std::string toString() const override {
				std::vector<std::string> str;
				for (size_t i = 0; i < child.size(); i++) {
					str.push_back(child[i]->toString());
				}
				sort(str.begin(), str.end());
				std::string res = const_sum == 0 ? "" : to_string(const_sum);
				for (size_t i = 0; i < child.size(); i++) {
					if (str[i][0] == '-' or res.empty()) {
						res += str[i];
					} else {
						res += '+' + str[i];
					}
				}
				if (res.find("+") != string::npos or res.find("-") != string::npos) {
					return "(" + res + ")";
				} else {
					return res;
				}
			}

			/**
			* @brief	�������
			* @return	���������
			*/
			PolyNode& addItem(ExprNode* item) {
				if (item->getType() == NodeType::consta) {
					const_sum += item->getValue();
				} else {
					child.push_back(item);
				}
				return *this;
			}
		};
	} // namespace expr
} // namespace mlib