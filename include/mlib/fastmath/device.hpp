#pragma once
/**
 * @file		device.hpp
 * @brief		并行加速设备抽象接口
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/fastmath.hpp"
namespace mlib {
	namespace fastmath {
		/**
		 * @brief				并行加速设备抽象接口
		 * @param RawDeviceType	原始设备类型
		 */
		template<typename RawDeviceType> class Device {
		public:
			RawDeviceType device;

			/**
			 * @brief	获取完整的设备信息字符串
			 * @details	包括设备类型(CPU/GPU)、专用内存、双精度浮点(fp32)是否可用
			 * @return	完整设备信息
			 */
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

			/**
			 * @brief	获取设备的简短说明
			 * @return	设备的简短说明
			 */
			virtual std::wstring getInfo() const = 0;

			// 获取设备专用内存
			/**
			 * @brief	获取设备专用内存
			 * @return	专用内存大小(KB)
			 */
			virtual size_t getMemKB() const = 0;

			/**
			 * @brief	双精度浮点(fp32)是否可用
			 * @return	判断结果
			 */
			virtual bool isFp32Available() const = 0;

			// 是否是GPU设备
			/**
			 * @brief	是否是GPU设备
			 * @return	判断结果
			 */
			virtual bool isGpu() const = 0;
		};
	} // namespace fastmath
} // namespace mlib