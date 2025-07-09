#pragma once
/**
 * @file		CSI.hpp
 * @brief		通过控制台虚拟终端序列 (CSI) 实现的功能
 * @details		https://learn.microsoft.com/zh-cn/windows/console/console-virtual-terminal-sequences
 * @note		以 Printer 形式提供的成员可以以函数形式调用, 例如：clear_line()
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "../printer.hpp"

namespace mlib {
	namespace Console {
		/** @brief 清空行, 回到行首 */			const Printer clearLine("\033[2K\r");
		/** @brief 清空屏幕, 回到屏幕原点 */	const Printer clearScreen("\033[1J\033[0;0H");

		/** @brief 字体重置 */		const Printer fReset("\033[0m");
		/** @brief 字体设为粗体 */	const Printer fBold("\033[1m");
		/** @brief 字体取消粗体 */	const Printer fNoBold("\033[22m");
		/** @brief 字体下划线 */		const Printer fUdl("\033[4m");
		/** @brief 字体取消下划线 */	const Printer fNoUdl("\033[24m");

		/** @brief 字体黑色 */	const Printer fBlack("\033[30m");
		/** @brief 字体红色 */	const Printer fRed("\033[31m");
		/** @brief 字体绿色 */	const Printer fGreen("\033[32m");
		/** @brief 字体黄色 */	const Printer fYellow("\033[33m");
		/** @brief 字体蓝色 */	const Printer fBlue("\033[34m");
		/** @brief 字体品红色 */	const Printer fMagenta("\033[35m");
		/** @brief 字体青色 */	const Printer fCyan("\033[36m");
		/** @brief 字体白色 */	const Printer fWhite("\033[37m");
	} // namespace console
} // namespace mlib