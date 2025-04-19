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
		inline win::CONSOLE_SCREEN_BUFFER_INFO get_screen_info() {
			using namespace win;
			CONSOLE_SCREEN_BUFFER_INFO bufferInfo = { 0 };
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);
			return bufferInfo;
		}
		/**
		* @brief	��ȡ��Ļ���
		* @return	��Ļ���(�ַ���)
		*/
		inline short get_width() {
			return get_screen_info().dwSize.X;
		}
		/**
		* @brief	��ȡ��Ļ�߶�
		* @return	��Ļ�߶�(�ַ���)
		*/
		inline short get_height() {
			return get_screen_info().dwSize.Y;
		}
	} // namespace console
} // namespace mlib