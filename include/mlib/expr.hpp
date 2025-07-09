#pragma once
/**
 * @file		expr.hpp
 * @brief		数学表达式树
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

/** 异常 */
const std::logic_error NodeAlreadyExist("ConstNode or VarNode already exists.");// 尝试创建已经存在的常数or变量节点
const std::out_of_range NodeNotExist("ConstNode or VarNode not exist.");		// 尝试访问不存在的常数or变量节点

/** 节点 */
#include "expr/node_expr.hpp"

#include "expr/node_const.hpp"
#include "expr/node_var.hpp"

#include "expr/node_mono.hpp"
#include "expr/node_poly.hpp"