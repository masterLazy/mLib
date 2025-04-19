#pragma once
/**
 * @file		logger.hpp
 * @brief		简单的日志类
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include <iostream>
#include <fstream>

#include "console.hpp"

namespace mlib {
	namespace logger {
		enum LogLevel {
			debug = 0,
			info = 1,
			warn = 2,
			error = 3
		};
		/**
		* @brief	简单的日志类
		*/
		class Logger {
			LogLevel log_level;
			std::ostream& os;
			bool color;

			void log(LogLevel log_level, const std::string& msg) const {
				if (log_level < this->log_level) return;
				switch (log_level) {
				case logger::debug:
					os << "[DEBUG";
					break;
				case logger::info:
					if (color) os << console::f_cyan;
					os << "[INFO";
					break;
				case logger::warn:
					if (color) os << console::f_yellow;
					os << "[WARN";
					break;
				case logger::error:
					if (color) os << console::f_red;
					os << "[ERROR";
					break;
				}
				os << "]\t" << msg << endl;
				if (color) os << console::f_reset;
			}
		public:
			/**
			* @param log_level	输出日志级别
			* @param os			要使用的输出流
			* @param color		是否使用彩色输出
			*/
			Logger(LogLevel log_level = logger::info, std::ostream& os = std::clog, bool color = true) :
				log_level(log_level), color(color), os(os) {
			}

			/** @brief 记录 debug 信息 */
			void debug(const std::string& msg) const {
				log(logger::debug, msg);
			}
			/** @brief 记录 info 信息 */
			void info(const std::string& msg) const {
				log(logger::info, msg);
			}
			/** @brief 记录 warn 信息 */
			void warn(const std::string& msg) const {
				log(logger::warn, msg);
			}
			/** @brief 记录 error 信息 */
			void error(const std::string& msg) const {
				log(logger::error, msg);
			}
		};
	} // namespace logger
} // namespace mlib