#pragma once
/*
* C++ AMP (Accelerated Massive Parallelism) 后端
* 并行计算设备
*
* 需要安装 VC++
* 警告: 从 Visual Studio 2022 版本 17.0 开始，已弃用 C++ AMP 标头。
*/
#include "../fastmath.hpp"
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