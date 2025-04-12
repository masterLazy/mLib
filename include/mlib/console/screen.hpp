#pragma once
/**
 * @file		screen.hpp
 * @brief		获取和操控控制台屏幕信息
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/console.hpp"

namespace mlib {
	namespace console {
		/// <summary>
		/// 获取屏幕信息
		/// </summary>
		inline win::CONSOLE_SCREEN_BUFFER_INFO get_screen_info() {
			using namespace win;
			CONSOLE_SCREEN_BUFFER_INFO bufferInfo = { 0 };
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);
			return bufferInfo;
		}
		/// <summary>
		/// 获取屏幕宽度(字符数)
		/// </summary>
		inline short get_width() {
			return get_screen_info().dwSize.X;
		}
		/// <summary>
		/// 获取屏幕高度(字符数)
		/// </summary>
		inline short get_height() {
			return get_screen_info().dwSize.Y;
		}
		/// <summary>
		/// 清空光标所在行
		/// </summary>
		inline void clear_line() {
			fputs("\033[2K\r", stdout);
			fflush(stdout);
		}
		/// <summary>
		/// 清空屏幕
		/// </summary>
		inline bool clear_screen() {
			using namespace win;
			/* https://learn.microsoft.com/zh-cn/windows/console/clearing-the-screen */
			HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
			if (!hStdOut) {
				return false;
			}
			CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
			if (!GetConsoleScreenBufferInfo(hStdOut, &bufferInfo)) {
				return false;
			}
			COORD coordScreen = { 0, 0 };
			DWORD cCharsWritten;
			DWORD dwConSize;
			dwConSize = get_width() * get_height();
			if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', dwConSize,
				coordScreen, &cCharsWritten)) {
				return false;
			}
			if (!FillConsoleOutputAttribute(hStdOut, bufferInfo.wAttributes, dwConSize,
				coordScreen, &cCharsWritten)) {
				return false;
			}
			SetConsoleCursorPosition(hStdOut, coordScreen);
			return true;
		}
	} // namespace console
} // namespace mlib