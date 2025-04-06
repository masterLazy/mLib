#pragma once
/*
* [����]
* ���ຯ��
*/

#include <iostream>
#include <sstream>		// ostringstream
#include <iomanip>		// setprecision()

namespace mlib {
	namespace funciton {
		/// <summary>
		/// �ֽ���ת�ַ���
		/// </summary>
		/// <param name="bytes">�ֽ���</param>
		/// <param name="precision">������λС��</param>
		/// <param name="i_style">�Ƿ�ʹ����"KiB"����ʽ</param>
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
			// �����ִ�е�������
			oss << std::setprecision(precision) << bytes / 1024 << (i_style ? "TiB" : "TB");
			return oss.str();
		}

	} // namespace function
} // namespace mlib