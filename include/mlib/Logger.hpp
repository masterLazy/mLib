#pragma once
/**
 * @file		Logger.hpp
 * @brief		日志器
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include <iostream>
#include <fstream>

#include "console.hpp"

namespace mlib {
	/**
	* @brief	日志器
	*/
	class Logger {
	public:
		enum LogLevel {
			debug = 0,
			info = 1,
			warn = 2,
			error = 3,
			fatal = 4
		};
	private:
		LogLevel log_level;
		std::ostream& os;
		bool color;

		void log(LogLevel log_level, const std::string& msg) const {
			if (log_level < this->log_level) return;
			switch (log_level) {
			case debug:
				os << "[DEBUG";
				break;
			case info:
				if (color) os << Console::fCyan;
				os << "[INFO";
				break;
			case warn:
				if (color) os << Console::fYellow;
				os << "[WARN";
				break;
			case error:
				if (color) os << Console::fRed;
				os << "[ERROR";
				break;
			case fatal:
				if (color) os << Console::fRed << Console::fUdl;
				os << "[FATAL";
				break;
			}
			os << "] " << msg << std::endl;
			if (color) os << Console::fReset;
		}
	public:
		/**
		* @param log_level	输出日志级别
		* @param os			要使用的输出流
		* @param color		是否使用彩色输出
		*/
		Logger(LogLevel log_level = info, std::ostream& os = std::clog, bool color = true) :
			log_level(log_level), color(color), os(os) {
		}

		/** @brief 调试信息 */
		void debug(const std::string& msg) const {
			log(debug, msg);
		}
		/** @brief 一般信息 */
		void info(const std::string& msg) const {
			log(info, msg);
		}
		/** @brief 警告 */
		void warn(const std::string& msg) const {
			log(warn, msg);
		}
		/** @brief 错误 */
		void error(const std::string& msg) const {
			log(error, msg);
		}
		/** @brief 致命错误 */
		void fatal(const std::string& msg) const {
			log(fatal, msg);
		}
	};
} // namespace logger
} // namespace mlib