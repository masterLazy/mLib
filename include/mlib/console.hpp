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

#include "console/screen.hpp"			// ��ȡ�Ͳٿؿ���̨��Ļ��Ϣ
#include "console/progressbar.hpp"		// ������