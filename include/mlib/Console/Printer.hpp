#pragma once
/**
 * @file		Printer.hpp
 * @brief		多种输出方式的字符串封装
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "../Console.hpp"

namespace mlib {
	namespace Console {
		/**
		* @brief	多种输出方式的字符串封装
		*/
		class Printer {
			std::string str;
		public:
			Printer(std::string str) :str(str) {}
			Printer(std::string pre, int i, std::string post) {
				str = pre + std::to_string(i) + post;
			}
			/** @brief	转换到字符串 */
			std::string toString() const {
				return str;
			}
			/** @brief	用<<重载打印 */
			friend std::ostream& operator<<(std::ostream& os, const Printer& self) {
				os << self.str;
				return os;
			}
			/** @brief	用cout打印 */
			void operator()() const {
				std::cout << str << std::flush;
			}
		};
	} // namespace console
} // namespace mlib