#pragma once
/*
* [组件头]
* Console: 用于控制台的实用工具
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

#include "console/screen.hpp"			// 获取和操控控制台屏幕信息
#include "console/progressbar.hpp"		// 进度条