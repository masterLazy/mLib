#pragma once
/**
 * @file		screen.hpp
 * @brief		��ȡ�Ͳٿؿ���̨��Ļ��Ϣ
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/console.hpp"

namespace mlib {
	namespace console {
		/**
		* @brief	��ȡ��Ļ��Ϣ
		*/
		inline CONSOLE_SCREEN_BUFFER_INFO getScreenInfo() {
			CONSOLE_SCREEN_BUFFER_INFO bufferInfo = { 0 };
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);
			return bufferInfo;
		}
		/**
		* @brief	��ȡ��Ļ���
		* @return	��Ļ���(�ַ���)
		*/
		inline short getWidth() {
			return getScreenInfo().dwSize.X;
		}
		/**
		* @brief	��ȡ��Ļ�߶�
		* @return	��Ļ�߶�(�ַ���)
		*/
		inline short getHeight() {
			return getScreenInfo().dwSize.Y;
		}
	} // namespace console
} // namespace mlib