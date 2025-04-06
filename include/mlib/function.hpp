#pragma once
/*
* [共享]
* 杂类函数
*/

#include <iostream>
#include <sstream>		// ostringstream
#include <iomanip>		// setprecision()

namespace mlib {
	namespace funciton {
		/// <summary>
		/// 字节数转字符串
		/// </summary>
		/// <param name="bytes">字节数</param>
		/// <param name="precision">保留几位小数</param>
		/// <param name="i_style">是否使用如"KiB"的样式</param>
		/// <returns></returns>
		std::string byte_to_string(float bytes, int precision = 2, bool i_style = true) {
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
			// 真的能执行到这里吗
			oss << std::setprecision(precision) << bytes / 1024 << (i_style ? "TiB" : "TB");
			return oss.str();
		}

	} // namespace function
} // namespace mlib