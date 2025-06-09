#pragma once
/**
 * @file		progressbar.hpp
 * @brief		进度条
 * @details		类tdqm样式
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/console.hpp"
#include "mlib/function.hpp"

namespace mlib {
	namespace console {
		const std::string __block_strs[9] = {
			"",			// 空
			"\u258f",	// 左 1/8
			"\u258e",	// 左 2/8
			"\u258d",	// 左 3/8
			"\u258c",	// 左 4/8
			"\u258b",	// 左 5/8
			"\u258a",	// 左 6/8
			"\u2589",	// 左 7/8
			"\u2588"	// 完整方块
		};
		/**
		 * @brief		带eta计算功能的进度条
		 */
		class ProgressBar {
			size_t current, total;
			clock_t begin_time;
			int width;
			std::string title;		// 末尾额外添加了一个空格
			bool is_bytes = false;	// 工作量的单位是字节
		public:
			/**
			 * @brief					创建一个进度条
			 * @param total_of_work		总工作量
			 * @param initial_position	初始位置, 即初始进度
			 * @param width				进度条的宽度, 默认(-1)则自动匹配控制台宽度
			 */
			ProgressBar(size_t total_of_work, size_t initial_position = 0, int width = -1) {
				current = initial_position;
				total = total_of_work;
				if (width == -1) {
					this->width = console::getWidth();
				} else {
					this->width = width;
				}
				begin_time = clock_t();
				title = "";
			}
			/**
			 * @brief					创建一个进度条
			 * @param title				进度条标题
			 * @param total_of_work		总工作量
			 * @param initial_position	初始位置, 即初始进度
			 * @param width				进度条的宽度, 默认(-1)则自动匹配控制台宽度
			 */
			ProgressBar(const std::string& title, size_t total_of_work, size_t initial_position = 0, int width = -1) {
				current = initial_position;
				total = total_of_work;
				if (width == -1) {
					this->width = console::getWidth();
				} else {
					this->width = width;
				}
				begin_time = clock_t();
				this->title = title + ' ';
			}

			/** @brief	重置进度条 */
			void reset() {
				current = 0;
				begin_time = clock();
			}
			/**
			 * @brief					增加进度
			 * @param work_of_forward	要增加的工作量
			 * @return					更改后的进度条自身
			 */
			ProgressBar& foward(int work_of_forward = 1) {
				if (current < total) {
					current = current + work_of_forward;
				} else {
					reset();
				}
				return *this;
			}
			/**
			 * @brief					设置进度
			 * @param current_work		当前工作量
			 */
			void setCurrent(size_t current_work) {
				current = current_work % total;
			}
			/**
			 * @brief					设置总工作量
			 * @param total_of_work		总工作量
			 */
			void setTotal(size_t total_of_work) {
				total = total_of_work;
			}
			/**
			 * @brief			设置宽度
			 * @param width		宽度
			 */
			void setWidth(size_t width) {
				this->width = width;
			}
			/** @brief		获取当前完成的工作量  */
			size_t getCurrent() const {
				return current;
			}
			/**  @brief		获取总工作量 */
			size_t getTotal() const {
				return total;
			}
			/**
			 * @brief		获取已用时间
			 * @return		已用时间(s)
			 */
			int getTimeSpent() const {
				return float(clock() - begin_time) / CLOCKS_PER_SEC;
			}
			/**  @brief		获取估计剩余时间(s) */
			int getEta() const {
				float sec = float(clock() - begin_time) / CLOCKS_PER_SEC;
				return sec / current * (total - current);
			}
			/** @brief		获取每秒完成的工作量 */
			float getItps() const {
				return current / getTimeSpent();
			}
			/**
			 * @brief		告知工作量的单位是字节
			 * @details		工作量将以字节数格式显示
			 */
			void workOfBytes(bool is_work_of_bytes) {
				is_bytes = is_work_of_bytes;
			}
			/**
			 * @brief		用cout打印进度条
			 * @return		进度条自身
			 */
			ProgressBar& print(bool clearLine = true) {
				if (clearLine) {
					// console::clearLine();
					std::cout << "\r";
				}
				std::cout << toString();
				return *this;
			}

			// 核心代码

			/**
			 * @brief		获取进度条字符串
			 * @example		100%|█████████| 128/128 [00:02<00:00, 58.55it/s]
			 */
			std::string toString() {
				std::string str;
				// xxx%|
				char pre[8] = { 0 };
				sprintf_s(pre, "%3d%%|", int(float(current) / total * 100));
				// | x/x [xx:xx<xx:xx, xx.xxit/s]
				// or | x/x [xx:xx<xx:xx, xx.xxs/it]
				char suf[512] = { 0 };
				float sec = float(clock() - begin_time) / CLOCKS_PER_SEC;
				size_t eta = sec / current * (total - current);
				if (is_bytes) {
					if (current == 0 or sec == 0 or eta < 0) {
						sprintf_s(suf, "| %s/%s [%02ld:%02ld<?, ?B/s]",
							funciton::ByteToString(current).c_str(),
							funciton::ByteToString(total).c_str(),
							int(sec) / 60, int(sec) % 60);
					} else { // _B/s
						sprintf_s(suf, "| %s/%s [%02d:%02ld<%02d:%02ld, %s/s]",
							funciton::ByteToString(current).c_str(),
							funciton::ByteToString(total).c_str(),
							int(sec) / 60, int(sec) % 60, eta / 60, eta % 60,
							funciton::ByteToString(current / sec).c_str());
					}
				} else {
					if (current == 0 or sec == 0 or eta < 0) {
						sprintf_s(suf, "| %ld/%ld [%02d:%02d<?, ?it/s]",
							current, total, int(sec) / 60, int(sec) % 60);
					} else if (current > sec) { // it/s
						sprintf_s(suf, "| %ld/%ld [%02d:%02d<%02d:%02d, %.2fit/s]",
							current, total, int(sec) / 60, int(sec) % 60, eta / 60, eta % 60, current / sec);
					} else { // s/it
						sprintf_s(suf, "| %ld/%ld [%02d:%02d<%02d:%02d, %.2fs/it]",
							current, total, int(sec) / 60, int(sec) % 60, eta / 60, eta % 60, sec / current);
					}
				}
				// ███████
				int width = this->width - title.size() - strnlen_s(pre, 8) - strnlen_s(suf, 128);
				width = max(width, 0);
				float blocks = float(current) / total * width;
				for (int i = 0; i < int(blocks); i++) {
					str += __block_strs[8];
				}
				str += __block_strs[int((blocks - int(blocks)) * 8)];
				for (int i = 0;
					i < width - int(blocks) - (int((blocks - int(blocks)) * 8) == 0 ? 0 : 1);
					i++) {
					str += " ";
				}
				str = title + pre + str + suf;
				return str;
			}
		};
	} // namespace console
} // namespace mlib