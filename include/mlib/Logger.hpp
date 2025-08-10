#pragma once
/**
 * @file		Logger.hpp
 * @brief		�򵥵���־��
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>

#include "Function.hpp"

namespace mlib {
	/**
	* @brief	�򵥵���־��
	*/
	class Logger {
	public:
		enum class Level {
			debug = 0,
			info = 1,
			warn = 2,
			error = 3,
			fatal = 4
		};
	private:
		std::ofstream of; // �ļ������
		Level logLevel;
		std::string filename;
		bool datetime;
		std::mutex mtx;

		void log(Level logLevel, const std::string& msg) {
			if (logLevel < this->logLevel) return;
			std::string full_msg;
			if (datetime) {
				full_msg += "[" + Function::GetFormatDateTime(true, true) + "] ";
			}
			full_msg += "[";
			switch (logLevel) {
			case Level::debug:
				full_msg += "DEBUG";
				break;
			case Level::info:
				full_msg += "INFO";
				break;
			case Level::warn:
				full_msg += "WARN";
				break;
			case Level::error:
				full_msg += "ERROR";
				break;
			case Level::fatal:
				full_msg += "FATAL";
				break;
			}
			full_msg += "]: " + msg + "\n";
			emit(full_msg);
		}
		void log(Level logLevel, const std::wstring& msg) {
			if (logLevel < this->logLevel) return;
			std::wstring full_msg;
			if (datetime) {
				full_msg += L"[" + Function::GetFormatDateTimeW(true, true) + L"] ";
			}
			full_msg += L"[";
			switch (logLevel) {
			case Level::debug:
				full_msg += L"DEBUG";
				break;
			case Level::info:
				full_msg += L"INFO";
				break;
			case Level::warn:
				full_msg += L"WARN";
				break;
			case Level::error:
				full_msg += L"ERROR";
				break;
			case Level::fatal:
				full_msg += L"FATAL";
				break;
			}
			full_msg += L"]: " + msg + L"\n";
			emit(full_msg);
		}
		void emit(std::string msg) {
			std::unique_lock<std::mutex> lock(mtx);
			std::clog << msg;
			if (not filename.empty()) {
				std::ofstream of(filename, std::ios::app);
				if (of) {
					of << msg;
					of.close();
				}
			}
		}
		void emit(std::wstring msg) {
			std::unique_lock<std::mutex> lock(mtx);
			std::wclog << msg;
			if (not filename.empty()) {
				std::wofstream of(filename, std::ios::app);
				if (of) {
					of << msg;
					of.close();
				}
			}
		}
	public:
		/**
		* @param logLevel	�����־����
		* @param dir		����ļ��� (���ձ�ʾ��ʹ���ļ���־)
		* @param datetime	�Ƿ�׷������-ʱ����Ϣ
		*/
		Logger(Level logLevel = Level::info, std::string filename = "", bool datetime = true) :
			logLevel(logLevel), filename(filename), datetime(datetime) {
			if (not filename.empty()) {
				std::ofstream f(filename, std::ios::ate); // ����ļ�
				f.close();
			}
		}

		/** @brief ������Ϣ */
		void debug(const std::string& msg) { log(Level::debug, msg); }
		void debug(const std::wstring& msg) { log(Level::debug, msg); }
		/** @brief һ����Ϣ */
		void info(const std::string& msg) { log(Level::info, msg); }
		void info(const std::wstring& msg) { log(Level::info, msg); }
		/** @brief ���� */
		void warn(const std::string& msg) { log(Level::warn, msg); }
		void warn(const std::wstring& msg) { log(Level::warn, msg); }
		/** @brief ���� */
		void error(const std::string& msg) { log(Level::error, msg); }
		void error(const std::wstring& msg) { log(Level::error, msg); }
		/** @brief �������� */
		void fatal(const std::string& msg) { log(Level::fatal, msg); }
		void fatal(const std::wstring& msg) { log(Level::fatal, msg); }
	};
} // namespace mlib