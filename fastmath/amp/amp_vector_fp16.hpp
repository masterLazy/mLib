#pragma once
/*
* C++ AMP (Accelerated Massive Parallelism) 后端
* 单精度浮点(fp16)向量
*
* 需要安装 VC++
* 警告: 从 Visual Studio 2022 版本 17.0 开始，已弃用 C++ AMP 标头。
*/
#include "../fastmath.hpp"
namespace mlib {
	namespace amp {
		class VectorFp16 : public fastmath::Vector<float, VectorFp16> {
			concurrency::array_view<float, 1> *gpuArray;
			std::vector<float> cpuArray;
			typedef VectorFp16 Vector;
		public:
			// 创建向量(在CPU上)
			VectorFp16(size_t size) {
				cpuArray.resize(size);
				gpuArray = new concurrency::array_view<float, 1>(size, cpuArray);
			}
			// 通过拷贝创建向量(在CPU上)
			VectorFp16(std::vector<float> source) {
				cpuArray = source;
				gpuArray = new concurrency::array_view<float, 1>(size(), cpuArray);
			}

			~VectorFp16() {
				delete gpuArray;
			}

			size_t size() const override {
				return cpuArray.size();
			}

			// 将数据加载到并行计算设备
			Vector& toDevice(Device device = getDefaultDevice(), Result* res = nullptr) {
				gpuArray->synchronize_to(device.device.create_view());
				return *this;
			}
			// 将数据同步到CPU
			Vector& toCpu(Result* res = nullptr) {
				gpuArray->synchronize();
				return *this;
			}


			Vector& nagate(Result* res = nullptr) override {
				return *this;
			}
			Vector& add(const Vector& another, Result* res = nullptr) override {
				return *this;
			}
			Vector& add(float f, Result* res = nullptr) override {
				return *this;
			}
			Vector& sub(const Vector& another, Result* res = nullptr) override {
				return *this;
			}
			Vector& sub(float f, Result* res = nullptr) override {
				return *this;
			}
			Vector& mul(const Vector& another, Result* res = nullptr) override {
				return *this;
			}
			Vector& mul(float f, Result* res = nullptr) override {
				return *this;
			}
			Vector& div(const Vector& another, Result* res = nullptr) override {
				return *this;
			}
			Vector& div(float f, Result* res = nullptr) override {
				return *this;
			}
			Vector& pow(const Vector& another, Result* res = nullptr) override {
				return *this;
			}
			Vector& pow(float f, Result* res = nullptr) override {
				return *this;
			}
		};
	} // namespace amp
} // namespace mlib