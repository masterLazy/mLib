#pragma once
/**
 * @file 		amp_device.hpp
 * @brief		AMP设备
 * @details		C++ AMP (Accelerated Massive Parallelism)
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 *
 * @note		需要安装VC++
 * @warning		从Visual Studio 2022版本17.0开始, 已弃用C++ AMP标头。
 */

#include "mlib/fastmath.hpp"
namespace mlib {
	namespace fastmath {
		namespace amp {
			class Device : public fastmath::Device<concurrency::accelerator> {
			public:
				Device() {}
				Device(Concurrency::accelerator _device) {
					device = _device;
				}

				std::wstring getInfo() const override {
					return device.description;
				}
				virtual size_t getMemKB() const override {
					return device.dedicated_memory;
				}
				bool isFp32Available() const override {
					return device.supports_double_precision;
				}
				bool isGpu() const override {
					return !device.is_emulated;
				}
			};
			// 获取所有设备
			std::vector<Device> getDevices() {
				using namespace concurrency;
				std::vector<Device> devices;
				std::vector<accelerator> accs = accelerator::get_all();
				for (auto i : accs) {
					devices.push_back(Device(i));
				}
				return devices;
			}
			// 获取默认设备
			Device getDefaultDevice() {
				return getDevices()[0];
			}
		} // namespace amp
	} // namespace fastmath
} // namespace mlib