#pragma once
/*
* [����ӿ�]
* ���м����豸
*/
#include "fastmath.hpp"
namespace mlib {
	namespace fastmath {
		template<typename RawDeviceType> class Device {
		public:
			RawDeviceType device;

			// ��ȡ����˵��
			std::wstring getFullInfo() const {
				std::wstring str = getInfo() + L", ";
				size_t mem = getMemKB();
				if (isGpu()) {
					str += L"GPU, ";
					if (mem < 1024) {
						str += std::to_wstring(mem) + L" KB VRAM, ";
					} else {
						mem /= 1024;
						if (mem < 1024) {
							str += std::to_wstring(mem) + L" MB VRAM, ";
						} else {
							mem /= 1024;
							if (mem < 1024) {
								str += std::to_wstring(mem) + L" GB VRAM, ";
							} else {
								mem /= 1024;
							}
						}
					}
				} else {
					str += L"CPU, ";
				}
				if (isFp32Available()) {
					str += L"fp32 available";
				} else {
					str += L"fp32 unavailable";
				}
				return str;
			}

			// ��ȡ�豸�ļ��˵��
			virtual std::wstring getInfo() const = 0;
			// ��ȡ�豸ר���ڴ�
			virtual size_t getMemKB() const = 0;
			// ˫���ȸ���(fp32)�Ƿ����
			virtual bool isFp32Available() const = 0;
			// �Ƿ���GPU�豸
			virtual bool isGpu() const = 0;
		};
	} // namespace fastmath
} // namespace mlib