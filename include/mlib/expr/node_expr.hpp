#pragma once
/**
 * @file		node_expr.hpp
 * @brief		表达式树节点(抽象接口)
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/expr.hpp"

namespace mlib {
	namespace expr {
		/**
		* @brief 节点基类
		*/
		enum class NodeType {
			consta,	// 常数
			var,	// 变量
			poly,	// 多项式
			mono	// 单项式
		};
		class ExprNode {
		protected:
			NodeType node_type;
		public:
			/** @brief 是否是结构节点 */
			virtual bool isStructNode() const = 0;
			/** @brief 对子树求值 */
			virtual Real getValue() const = 0;
			/** @brief 转为字符串 */
			virtual std::string toString() const = 0;

			/** @brief 获取节点类型 */
			NodeType getType() {
				return node_type;
			}
		};
	} // namespace expr
} // namespace mlib