#pragma once
/*
* [���ͷ]
* Console: ���ڿ���̨��ʵ�ù���
*/

#include <cstdio>
#include <string>
#include <sstream>
// �� Windows.h Ҫ���ó��������
namespace win {
#include <Windows.h>
}
#undef min
#undef max

namespace mlib {
	namespace console {
		class Console {
			win::HANDLE hStdOut;
			win::CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
			bool fetch_bufferinfo() {
				return win::GetConsoleScreenBufferInfo(hStdOut, &bufferInfo);
			}
		public:
			Console(win::HANDLE hStdOut = NULL) {
				init(hStdOut);
			}

			bool init(win::HANDLE hStdOut = NULL) {
				using namespace win;
				if (!hStdOut) {
					hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
					if (!hStdOut) {
						return false;
					}
				}
				this->hStdOut = hStdOut;
				return win::GetConsoleScreenBufferInfo(hStdOut, &bufferInfo);
			}

			short get_width() const {
				return bufferInfo.dwSize.X;
			}
			short get_height() const {
				return bufferInfo.dwSize.Y;
			}
			win::COORD get_screen_size() const {
				return bufferInfo.dwSize;
			}
			win::HANDLE get_stdout_handle() const {
				return hStdOut;
			}
			win::CONSOLE_SCREEN_BUFFER_INFO get_screen_buffer_info() const {
				return bufferInfo;
			}

			bool set_width(short width) {
				bufferInfo.dwSize.X = width;
				return SetConsoleScreenBufferSize(hStdOut, bufferInfo.dwSize);
			}
			bool set_height(short height) {
				bufferInfo.dwSize.Y = height;
				return SetConsoleScreenBufferSize(hStdOut, bufferInfo.dwSize);
			}
			bool set_screen_size(const win::COORD size) {
				bufferInfo.dwSize = size;
				return SetConsoleScreenBufferSize(hStdOut, bufferInfo.dwSize);
			}
			bool set_screen_size(short width, short height) {
				return set_screen_size(win::COORD({ width, height }));
			}

			bool clear_screen() {
				using namespace win;
				/* https://learn.microsoft.com/zh-cn/windows/console/clearing-the-screen */
				COORD coordScreen = { 0, 0 };
				DWORD cCharsWritten;
				DWORD dwConSize;
				dwConSize = get_width() * get_height();
				if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', dwConSize,
					coordScreen, &cCharsWritten)) {
					return false;
				}
				if (!FillConsoleOutputAttribute(hStdOut, bufferInfo.wAttributes, dwConSize,
					coordScreen, &cCharsWritten)) {
					return false;
				}
				SetConsoleCursorPosition(hStdOut, coordScreen);
				return true;
			}
		};
	} // namespace console
} // namespace mlib

#include "progressbar.hpp"		// ������