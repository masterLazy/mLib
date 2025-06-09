#pragma once
/**
 * @file		fuction.hpp
 * @brief		杂项函数
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include <iostream>
#include <sstream>		// ostringstream
#include <iomanip>		// setprecision()
#include <sys/stat.h>	// stat

namespace mlib {
	namespace function {
		/**
		 * @brief				字符数转字符串
		 * @param bytes			字节数
		 * @param precision		保留几位小数
		 * @param i_style		是否使用如"KiB"的样式
		 */
		inline std::string ByteToString(float bytes, int precision = 2, bool i_style = true) {
			std::ostringstream oss;
			oss.setf(std::ios::fixed, std::ios::floatfield);
			if (bytes <= 999) {
				oss << std::setprecision(precision) << bytes << "B";
				return oss.str();
			}
			bytes /= 1024;
			if (bytes <= 999) {
				oss << std::setprecision(precision) << bytes << (i_style ? "KiB" : "KB");
				return oss.str();
			}
			bytes /= 1024;
			if (bytes <= 999) {
				oss << std::setprecision(precision) << bytes << (i_style ? "MiB" : "MB");
				return oss.str();
			}
			bytes /= 1024;
			if (bytes <= 999) {
				oss << std::setprecision(precision) << bytes << (i_style ? "GiB" : "GB");
				return oss.str();
			}
			oss << std::setprecision(precision) << bytes / 1024 << (i_style ? "TiB" : "TB");
			return oss.str();
		}

		/**
		 * @brief			获取文件大小
		 * @param file		要获取大小的文件指针
		 */
		inline size_t GetFileSize(FILE* file) {
			size_t off = _ftelli64(file);
			_fseeki64(file, -1, 0);
			size_t size = _ftelli64(file);
			_fseeki64(file, 0, off);
			return size;
		}
	} // namespace function
} // namespace mlib