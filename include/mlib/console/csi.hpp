#pragma once
/**
 * @file		csi.hpp
 * @brief		ͨ������̨�����ն����� (CSI) ʵ�ֵĹ���
 * @details		https://learn.microsoft.com/zh-cn/windows/console/console-virtual-terminal-sequences
 * @note		�� Printer ��ʽ�ṩ�ĳ�Ա�����Ժ�����ʽ����, ���磺clear_line()
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/console.hpp"

namespace mlib {
	namespace console {
		/** @brief �����, �ص����� */			const Printer clear_line("\033[2K\r");
		/** @brief �����Ļ, �ص���Ļԭ�� */	const Printer clear_screen("\033[1J\033[0;0H");

		/** @brief �������� */		const Printer f_reset("\033[0m");
		/** @brief ������Ϊ���� */	const Printer f_bold("\033[1m");
		/** @brief ����ȡ������ */	const Printer f_no_bold("\033[22m");
		/** @brief �����»��� */		const Printer f_udl("\033[4m");
		/** @brief ����ȡ���»��� */	const Printer f_no_udl("\033[24m");

		/** @brief �����ɫ */	const Printer f_black("\033[30m");
		/** @brief �����ɫ */	const Printer f_red("\033[31m");
		/** @brief ������ɫ */	const Printer f_green("\033[32m");
		/** @brief �����ɫ */	const Printer f_yellow("\033[33m");
		/** @brief ������ɫ */	const Printer f_blue("\033[34m");
		/** @brief ����Ʒ��ɫ */	const Printer f_magenta("\033[35m");
		/** @brief ������ɫ */	const Printer f_cyan("\033[36m");
		/** @brief �����ɫ */	const Printer f_white("\033[37m");
	} // namespace console
} // namespace mlib