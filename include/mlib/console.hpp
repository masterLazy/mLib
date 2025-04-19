#pragma once
/**
 * @file		console.hpp
 * @brief		用于控制台的实用工具
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include <cstdio>
#include <string>
#include <sstream>
 // 对 Windows.h 要做好充足防御。
namespace win {
#include <Windows.h>
}
#undef min
#undef max

#include "console/printer.hpp"

#include "console/csi.hpp"
#include "console/screen.hpp"

#include "console/progressbar.hpp"