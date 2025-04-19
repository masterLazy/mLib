#pragma once
/**
 * @file		logger.hpp
 * @brief		�򵥵���־��
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
		* @brief	�򵥵���־��
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
			* @param log_level	�����־����
			* @param os			Ҫʹ�õ������
			* @param color		�Ƿ�ʹ�ò�ɫ���
			*/
			Logger(LogLevel log_level = logger::info, std::ostream& os = std::clog, bool color = true) :
				log_level(log_level), color(color), os(os) {
			}

			/** @brief ��¼ debug ��Ϣ */
			void debug(const std::string& msg) const {
				log(logger::debug, msg);
			}
			/** @brief ��¼ info ��Ϣ */
			void info(const std::string& msg) const {
				log(logger::info, msg);
			}
			/** @brief ��¼ warn ��Ϣ */
			void warn(const std::string& msg) const {
				log(logger::warn, msg);
			}
			/** @brief ��¼ error ��Ϣ */
			void error(const std::string& msg) const {
				log(logger::error, msg);
			}
		};
	} // namespace logger
} // namespace mlib