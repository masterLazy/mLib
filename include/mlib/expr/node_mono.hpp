#pragma once
/**
 * @file		node_mono.hpp
 * @brief		单项式节点
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/expr.hpp"

namespace mlib {
	namespace expr {
		/**
		* @brief 单项式节点
		*/
		class MonoNode : public ExprNode {
			Real coe = 1;					// 系数
			std::vector<ExprNode*> child;	// 子项(不包含常数)
			std::vector<ExprNode*> exp;		// 子项的指数
		public:
			MonoNode() {
				this->node_type = NodeType::mono;
			}
			/** @brief 是否是结构节点 */
			bool isStructNode() const override {
				return true;
			}
			/** @brief 对子树求值 */
			Real getValue() const override {
				Real sum = coe;
				for (size_t i = 0; i < child.size(); i++) {
					sum *= pow(child[i]->getValue(), exp[i]->getValue());
				}
				return sum;
			}
			/** @brief 转为字符串 */
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
			* @brief	添加子项
			* @return	自身的引用
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