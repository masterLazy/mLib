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
#include <exception>

namespace mlib {
	/**
	 * @brief	��װһ�����̣����Զ������ Std I/O ����
	 */
	class Process {
		HANDLE to_hRead = NULL, to_hWrite = NULL;
		HANDLE from_hRead = NULL, from_hWrite = NULL;
		PROCESS_INFORMATION process_info;
	public:
		/**
		* @brief				ʵ��������
		* @param file_name		Ҫִ�е��ļ� (����Ϊ��)
		* @param command_line	Ҫִ�е�������
		* @param show_window	�Ƿ���ʾ����
		* @exception std::exception �˺������׳��쳣��
		*/
		Process(
			const std::string& file_name,
			const std::string& command_line,
			const std::string& working_dir = "",
			bool show_window = false) {
			// �����ܵ�
			SECURITY_ATTRIBUTES se = { 0 };
			se.lpSecurityDescriptor = NULL;
			se.bInheritHandle = TRUE;
			se.nLength = sizeof(se);
			if (CreatePipe(&to_hRead, &to_hWrite, &se, 0) == 0) {
				throw std::exception("Failed to create pipe");
			}
			if (CreatePipe(&from_hRead, &from_hWrite, &se, 0) == 0) {
				throw std::exception("Failed to create pipe");
			}
			// ��������
			STARTUPINFOA sInfo = { 0 };
			sInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			sInfo.wShowWindow = show_window ? SW_SHOW : SW_HIDE;
			sInfo.hStdInput = to_hRead;
			sInfo.hStdOutput = from_hWrite;
			sInfo.hStdError = from_hWrite;
			if (CreateProcessA(
				file_name.empty() ? NULL : file_name.c_str(),
				command_line.empty() ? NULL : (char*)command_line.c_str(),
				NULL, NULL, TRUE, NULL, NULL,
				working_dir.empty() ? NULL : working_dir.c_str(),
				&sInfo, &process_info) == 0) {
				throw std::exception("Failed to create process");
			}
		}
		~Process() {
			if (getExitCode() == STILL_ACTIVE) {
				kill();
			}
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
		* @exception std::exception д��ʧ��ʱ�׳��쳣
		*/
		DWORD write(const std::string& data) {
			DWORD bytes_written;
			if (WriteFile(to_hWrite, data.c_str(), data.size(), &bytes_written, NULL) == 0) {
				throw std::exception("Failed to write to pipe");
			}
			return bytes_written;
		}
		/**
		* @brief	���� stdout �е��ֽ���(��ɾ��)
		* @return	stdout �е��ֽ���
		* @exception std::exception peek ʧ��ʱ�׳��쳣
		*/
		DWORD peek() {
			DWORD bytes;
			if (PeekNamedPipe(from_hRead, NULL, NULL, NULL, &bytes, NULL) == 0) {
				throw std::exception("Failed to peek pipe");
			}
			return bytes;
		}
		/**
		* @brief				�ӽ��̵� stdout ��ȡ����
		* @param bytes_to_read	Ҫ��ȡ������ֽ���
		* @return				ʵ�ʶ�ȡ���ֽ���
		* @exception std::exception ��ȡʧ��ʱ�׳��쳣
		*/
		std::string read(DWORD bytes_to_read = 512) {
			DWORD bytes_read;
			unsigned char* buf = new unsigned char[bytes_to_read];
			if (ReadFile(from_hRead, buf, bytes_to_read, &bytes_read, NULL) == 0) {
				delete[] buf;
				throw std::exception("Failed to read from pipe");
			}
			std::string res;
			res.resize(bytes_read);
			for (DWORD i = 0; i < bytes_read; i++) {
				res[i] = buf[i];
			}
			delete[] buf;
			return res;
		}
		/**
		* @brief				�ӽ��̵� stdout ��ȡ����ֱ����β
		* @param max_bytes		Ҫ��ȡ������ֽ���
		* @return				ʵ�ʶ�ȡ���ֽ���
		* @exception std::exception ��ȡʧ��ʱ�׳��쳣
		*/
		std::string getline(DWORD max_bytes = 1024) {
			DWORD bytes_read;
			std::string res;
			char buf;
			for (DWORD i = 0; i < max_bytes; i++) {
				if (ReadFile(from_hRead, &buf, 1, &bytes_read, NULL) == 0) {
					throw std::exception("Failed to read from pipe");
				}
				if (buf == '\r' || buf == '\n') {
					if (res.empty()) {
						continue;
					} else return res;
				}
				res += buf;
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
			TerminateProcess(process_info.hProcess, exit_code);
		}
	};
} // namespace  mlib