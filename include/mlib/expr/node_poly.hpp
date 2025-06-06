#pragma once
/**
 * @file		node_poly.hpp
 * @brief		多项式节点
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/expr.hpp"

namespace mlib {
	namespace expr {
		/**
		* @brief 多项式节点
		*/
		class PolyNode : public ExprNode {
			Real const_sum = 0;				// 所有常数
			std::vector<ExprNode*> child;	// 子项(不包含常数)
		public:
			PolyNode() {
				this->node_type = NodeType::poly;
			}
			/** @brief 是否是结构节点 */
			bool isStructNode() const override {
				return true;
			}
			/** @brief 对子树求值 */
			Real getValue() const override {
				Real sum = const_sum;
				for (auto p : child) {
					sum += p->getValue();
				}
				return sum;
			}
			/** @brief 转为字符串 */
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
			* @brief	添加子项
			* @return	自身的引用
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