#pragma once
/**
 * @file		Printer.hpp
 * @brief		���������ʽ���ַ�����װ
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "../Console.hpp"

namespace mlib {
	namespace Console {
		/**
		* @brief	���������ʽ���ַ�����װ
		*/
		class Printer {
			std::string str;
		public:
			Printer(std::string str) :str(str) {}
			Printer(std::string pre, int i, std::string post) {
				str = pre + std::to_string(i) + post;
			}
			/** @brief	ת�����ַ��� */
			std::string toString() const {
				return str;
			}
			/** @brief	��<<���ش�ӡ */
			friend std::ostream& operator<<(std::ostream& os, const Printer& self) {
				os << self.str;
				return os;
			}
			/** @brief	��cout��ӡ */
			void operator()() const {
				std::cout << str << std::flush;
			}
		};
	} // namespace console
} // namespace mlib