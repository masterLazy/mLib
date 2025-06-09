#pragma once
/**
 * @file		node_expr.hpp
 * @brief		���ʽ���ڵ�(����ӿ�)
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/expr.hpp"

namespace mlib {
	namespace expr {
		/**
		* @brief �ڵ����
		*/
		enum class NodeType {
			consta,	// ����
			var,	// ����
			poly,	// ����ʽ
			mono	// ����ʽ
		};
		class ExprNode {
		protected:
			NodeType node_type;
		public:
			/** @brief �Ƿ��ǽṹ�ڵ� */
			virtual bool isStructNode() const = 0;
			/** @brief ��������ֵ */
			virtual Real getValue() const = 0;
			/** @brief תΪ�ַ��� */
			virtual std::string toString() const = 0;

			/** @brief ��ȡ�ڵ����� */
			NodeType getType() {
				return node_type;
			}
		};
	} // namespace expr
} // namespace mlib