#pragma once
/**
 * @file		CSI.hpp
 * @brief		ͨ������̨�����ն����� (CSI) ʵ�ֵĹ���
 * @details		https://learn.microsoft.com/zh-cn/windows/console/console-virtual-terminal-sequences
 * @note		�� Printer ��ʽ�ṩ�ĳ�Ա�����Ժ�����ʽ����, ���磺clear_line()
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "../printer.hpp"

namespace mlib {
	namespace Console {
		/** @brief �����, �ص����� */			const Printer clearLine("\033[2K\r");
		/** @brief �����Ļ, �ص���Ļԭ�� */	const Printer clearScreen("\033[1J\033[0;0H");

		/** @brief �������� */		const Printer fReset("\033[0m");
		/** @brief ������Ϊ���� */	const Printer fBold("\033[1m");
		/** @brief ����ȡ������ */	const Printer fNoBold("\033[22m");
		/** @brief �����»��� */		const Printer fUdl("\033[4m");
		/** @brief ����ȡ���»��� */	const Printer fNoUdl("\033[24m");

		/** @brief �����ɫ */	const Printer fBlack("\033[30m");
		/** @brief �����ɫ */	const Printer fRed("\033[31m");
		/** @brief ������ɫ */	const Printer fGreen("\033[32m");
		/** @brief �����ɫ */	const Printer fYellow("\033[33m");
		/** @brief ������ɫ */	const Printer fBlue("\033[34m");
		/** @brief ����Ʒ��ɫ */	const Printer fMagenta("\033[35m");
		/** @brief ������ɫ */	const Printer fCyan("\033[36m");
		/** @brief �����ɫ */	const Printer fWhite("\033[37m");
	} // namespace console
} // namespace mlib