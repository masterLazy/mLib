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
		/**
		* @brief	获取屏幕信息
		*/
		inline CONSOLE_SCREEN_BUFFER_INFO getScreenInfo() {
			CONSOLE_SCREEN_BUFFER_INFO bufferInfo = { 0 };
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);
			return bufferInfo;
		}
		/**
		* @brief	获取屏幕宽度
		* @return	屏幕宽度(字符数)
		*/
		inline short getWidth() {
			return getScreenInfo().dwSize.X;
		}
		/**
		* @brief	获取屏幕高度
		* @return	屏幕高度(字符数)
		*/
		inline short getHeight() {
			return getScreenInfo().dwSize.Y;
		}
	} // namespace console
} // namespace mlib