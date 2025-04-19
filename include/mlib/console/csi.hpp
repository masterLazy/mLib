#pragma once
/**
 * @file		csi.hpp
 * @brief		通过控制台虚拟终端序列 (CSI) 实现的功能
 * @details		https://learn.microsoft.com/zh-cn/windows/console/console-virtual-terminal-sequences
 * @note		以 Printer 形式提供的成员可以以函数形式调用, 例如：clear_line()
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/console.hpp"

namespace mlib {
	namespace console {
		/** @brief 清空行, 回到行首 */			const Printer clear_line("\033[2K\r");
		/** @brief 清空屏幕, 回到屏幕原点 */	const Printer clear_screen("\033[1J\033[0;0H");

		/** @brief 字体重置 */		const Printer f_reset("\033[0m");
		/** @brief 字体设为粗体 */	const Printer f_bold("\033[1m");
		/** @brief 字体取消粗体 */	const Printer f_no_bold("\033[22m");
		/** @brief 字体下划线 */		const Printer f_udl("\033[4m");
		/** @brief 字体取消下划线 */	const Printer f_no_udl("\033[24m");

		/** @brief 字体黑色 */	const Printer f_black("\033[30m");
		/** @brief 字体红色 */	const Printer f_red("\033[31m");
		/** @brief 字体绿色 */	const Printer f_green("\033[32m");
		/** @brief 字体黄色 */	const Printer f_yellow("\033[33m");
		/** @brief 字体蓝色 */	const Printer f_blue("\033[34m");
		/** @brief 字体品红色 */	const Printer f_magenta("\033[35m");
		/** @brief 字体青色 */	const Printer f_cyan("\033[36m");
		/** @brief 字体白色 */	const Printer f_white("\033[37m");
	} // namespace console
} // namespace mlib