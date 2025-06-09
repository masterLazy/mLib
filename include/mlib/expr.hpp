#pragma once
/**
 * @file		expr.hpp
 * @brief		��ѧ���ʽ��
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include <cmath>
#include <vector>
#include <unordered_map>
#include <exception>
#include <string>
#include <algorithm>

#include "expr/real.hpp"

/** �쳣 */
const std::logic_error NodeAlreadyExist("ConstNode or VarNode already exists.");// ���Դ����Ѿ����ڵĳ���or�����ڵ�
const std::out_of_range NodeNotExist("ConstNode or VarNode not exist.");		// ���Է��ʲ����ڵĳ���or�����ڵ�

/** �ڵ� */
#include "expr/node_expr.hpp"

#include "expr/node_const.hpp"
#include "expr/node_var.hpp"

#include "expr/node_mono.hpp"
#include "expr/node_poly.hpp"