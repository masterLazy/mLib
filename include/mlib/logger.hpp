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
#include <vector>

#include "console.hpp"

namespace mlib {
	namespace logger {
		enum LogLevel {
			debug = 0,
			info = 1,
			warn = 2,
			error = 3,
			fatal = 4
		};
		/**
		* @brief	简单的日志类
		*/
		class Logger {
			LogLevel log_level;
			std::ostream& os;
			std::vector<std::string> filenames;
			bool color;

			void log(LogLevel log_level, const std::string& msg) const {
				if (log_level < this->log_level) return;
				switch (log_level) {
				case logger::debug:
					dispatch("[DEBUG] ");
					break;
				case logger::info:
					if (color) os << console::fCyan;
					dispatch("[INFO] ");
					break;
				case logger::warn:
					if (color) os << console::fYellow;
					dispatch("[WARN] ");
					break;
				case logger::error:
					if (color) os << console::fRed;
					dispatch("[ERROR]");
					break;
				case logger::fatal:
					if (color) os << console::fRed << console::fUdl;
					dispatch("[FATAL]");
					break;
				}
				dispatch(" ").dispatch(msg).dispatch("\n");
				if (color) os << console::fReset;
			}
			const Logger& dispatch(std::string msg) const {
				os << msg;
				std::ofstream of;
				for (auto filename : filenames) {
					of.open(filename);
					if (not of.is_open())continue; // 忽略这个输出目标
					of << msg;
					of.close();
				}
				return *this;
			}
		public:
			/**
			* @param log_level	输出日志级别
			* @param os			要使用的输出流
			* @param color		是否使用彩色输出
			*/
			Logger(LogLevel log_level = logger::info, std::ostream& os = std::clog, bool color = false) :
				log_level(log_level), color(color), os(os) {
			}

			/**
			 * @brief 			添加文件输出
			 * @param filename	要写入的文件名
			 */
			bool addFileSink(std::string filename) {
				filenames.push_back(filename);
			}

			/** @brief 调试信息 */
			void debug(const std::string& msg) const {
				log(logger::debug, msg);
			}
			/** @brief 一般信息 */
			void info(const std::string& msg) const {
				log(logger::info, msg);
			}
			/** @brief 警告 */
			void warn(const std::string& msg) const {
				log(logger::warn, msg);
			}
			/** @brief 错误 */
			void error(const std::string& msg) const {
				log(logger::error, msg);
			}
			/** @brief 致命错误 */
			void fatal(const std::string& msg) const {
				log(logger::fatal, msg);
			}
		};
	} // namespace logger
} // namespace mlib