#pragma once
/**
 * @file		Process.hpp
 * @brief		��װһ�����̣����Զ������ Std I/O ����
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include <Windows.h>
#undef min
#undef max
#include <string>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <deque>

namespace mlib {
	/**
	 * @brief	��װһ�����̣����Զ������ Std I/O ����
	 */
	class Process {
		HANDLE to_hRead = NULL, to_hWrite = NULL;
		HANDLE from_hRead = NULL, from_hWrite = NULL;
		PROCESS_INFORMATION process_info;
		// ��ȡ�߳�
		std::thread* readThread;
		int signal;
		const int SG_NULL = 0;
		const int SG_EXIT = 1;
		std::mutex mtx;
		std::deque<char> readBuffer;
		void readThreadHandler() {
			DWORD bytes_read;
			char buf;
			while (signal != SG_EXIT) {
				if (ReadFile(from_hRead, &buf, 1, &bytes_read, NULL) && bytes_read) {
					std::unique_lock<std::mutex> lk(mtx);
					readBuffer.push_back(buf);
				}
			}
		}
	public:
		/**
		* @brief				ʵ��������
		* @param command_line	Ҫִ�е�������
		* @param show_window	�Ƿ�Ҫ��ʾ����
		* @exception std::runtime_error �˺������׳��쳣��
		*/
		Process(const std::string& command_line, bool show_window = false) {
			// �����ܵ�
			SECURITY_ATTRIBUTES se = { 0 };
			se.lpSecurityDescriptor = NULL;
			se.bInheritHandle = TRUE;
			se.nLength = sizeof(se);
			if (CreatePipe(&to_hRead, &to_hWrite, &se, 0) == 0) {
				throw std::runtime_error("Failed to create pipe");
			}
			if (CreatePipe(&from_hRead, &from_hWrite, &se, 0) == 0) {
				throw std::runtime_error("Failed to create pipe");
			}
			// ��������
			STARTUPINFOA sInfo = { 0 };
			sInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			sInfo.wShowWindow = show_window ? SW_SHOW : SW_HIDE;
			sInfo.hStdInput = to_hRead;
			sInfo.hStdOutput = from_hWrite;
			sInfo.hStdError = from_hWrite;
			if (CreateProcessA(NULL, (char*)command_line.c_str(), NULL, NULL, TRUE, NULL, NULL, NULL, &sInfo, &process_info) == 0) {
				throw std::runtime_error("Failed to create process");
			}
			// ������ȡ�߳�
			readThread = new std::thread(&Process::readThreadHandler, this);
			signal = NULL;
			readThread->detach();
		}
		~Process() {
			// ֪ͨ��ȡ�߳��˳�
			clock_t timer = clock();
			signal = SG_EXIT;
			while (signal == SG_EXIT && clock() - timer < 100);
			if (signal != SG_NULL) {
				TerminateThread(readThread->native_handle(), 0);
			}
			// �رվ��
			if (to_hWrite) {
				CloseHandle(to_hWrite);
			}
			if (from_hRead) {
				CloseHandle(from_hRead);
			}
			CloseHandle(process_info.hProcess);
		}
		/**
		* @brief	����̵� stdin д������
		* @return	ʵ��д����ֽ���
		* @exception std::runtime_error д��ʧ��ʱ�׳��쳣
		*/
		DWORD write(const std::string& data) {
			DWORD bytes_written;
			if (WriteFile(to_hWrite, data.c_str(), data.size(), &bytes_written, NULL) == 0) {
				throw std::runtime_error("Failed to write to pipe");
			}
			return bytes_written;
		}
		/**
		* @brief	���� stdout �е��ֽ���(��ɾ��)
		* @return	stdout �е��ֽ���
		* @exception std::runtime_error peek ʧ��ʱ�׳��쳣
		*/
		DWORD peek() {
			return readBuffer.size();
		}
		/**
		* @brief				�ӽ��̵� stdout ��ȡ����
		* @param bytes_to_read	Ҫ��ȡ������ֽ���
		* @return				ʵ�ʶ�ȡ���ֽ���
		*/
		std::string read(DWORD bytes_to_read = 512) {
			std::string res;
			while (!readBuffer.empty() && res.size() < bytes_to_read) {
				std::unique_lock<std::mutex> lk(mtx);
				res += readBuffer.front(), readBuffer.pop_front();
			}
			return res;
		}
		/**
		* @brief				�ӽ��̵� stdout ��ȡ����ֱ����β
		* @param max_bytes		Ҫ��ȡ������ֽ���
		* @return				ʵ�ʶ�ȡ���ֽ���
		*/
		std::string readLine(DWORD max_bytes = 1024) {
			std::string res;
			// ���˻��з�
			while (!readBuffer.empty() && (readBuffer.front() == '\r' || readBuffer.front() == '\n')) {
				std::unique_lock<std::mutex> lk(mtx);
				readBuffer.pop_front();
			}
			while (!readBuffer.empty() && res.size() < max_bytes) {
				std::unique_lock<std::mutex> lk(mtx);
				if (readBuffer.front() == '\r' || readBuffer.front() == '\n') break;
				res += readBuffer.front(), readBuffer.pop_front();
			}
			return res;
		}
		/**
		* @brief				��ȡ���̵ķ���ֵ
		* @retval STILL_ACTIVE	����δ�˳�
		*/
		DWORD getExitCode() {
			DWORD code;
			GetExitCodeProcess(process_info.hProcess, &code);
			return code;
		}
		/**
		* @brief	ǿ����ֹ����
		*/
		void kill(UINT exit_code = 0) {
			signal = SG_EXIT;
			TerminateProcess(process_info.hProcess, exit_code);
		}
	};
} // namespace  mlib