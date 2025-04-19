#pragma once
/**
 * @file 		amp_fp16.hpp
 * @brief		?????(fp16)??
 * @details		C++ AMP (Accelerated Massive Parallelism)
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 *
 * @note		????VC++
 * @warning		?Visual Studio 2022??17.0??, ???C++ AMP???
 */

#include "mlib/fastmath.hpp"
namespace mlib {
	namespace amp {
		class VectorFp16 : public fastmath::Vector<float, VectorFp16> {
			concurrency::array_view<float, 1>* gpuArray;
			std::vector<float> cpuArray;
			typedef VectorFp16 Vector;
		public:
			// ????(?CPU?)
			VectorFp16(size_t size) {
				cpuArray.resize(size);
				gpuArray = new concurrency::array_view<float, 1>(size, cpuArray);
			}
			// ????????(?CPU?)
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

			// ????????????
			Vector& toDevice(Device device = getDefaultDevice(), Result* res = nullptr) {
				gpuArray->synchronize_to(device.device.create_view());
				return *this;
			}
			// ??????CPU
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