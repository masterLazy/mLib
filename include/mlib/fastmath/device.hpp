#pragma once
/*
* [抽象接口]
* 并行加速设备
*/
#include "fastmath.hpp"
namespace mlib {
	namespace fastmath {
		template<typename RawDeviceType> class Device {
		public:
			RawDeviceType device;

			// 获取完整说明
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

			// 获取设备的简短说明
			virtual std::wstring getInfo() const = 0;
			// 获取设备专用内存
			virtual size_t getMemKB() const = 0;
			// 双精度浮点(fp32)是否可用
			virtual bool isFp32Available() const = 0;
			// 是否是GPU设备
			virtual bool isGpu() const = 0;
		};
	} // namespace fastmath
} // namespace mlib