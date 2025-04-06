#pragma once
/*
* 进度条
*/

#include "mlib/console.hpp"
#include "mlib/function.hpp"

namespace mlib {
	namespace console {
		const std::string block_strs[9] = {
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
		/// <summary>
		/// 进度条，带 eta 计算功能
		/// </summary>
		class ProgressBar {
			size_t current, total;
			clock_t begin_time;
			int width;
			std::string title;		// 末尾额外添加了一个空格
			bool is_bytes = false;	// 工作量的单位是字节
		public:
			/// <summary>
			/// 创建一个进度条
			/// </summary>
			/// <param name="total_of_work">总工作量</param>
			/// <param name="initial_position">初始位置，即初始进度</param>
			/// <param name="width">进度条的宽度，默认(-1)则自动匹配控制台宽度</param>
			ProgressBar(size_t total_of_work, size_t initial_position = 0, int width = -1) {
				current = initial_position;
				total = total_of_work;
				if (width == -1) {
					this->width = Console().get_width();
				} else {
					this->width = width;
				}
				begin_time = clock_t();
				title = "";
			}
			/// <summary>
			/// 创建一个进度条
			/// </summary>
			/// <param name="title">进度条标题</param>
			/// <param name="total_of_work">总工作量</param>
			/// <param name="initial_position">初始位置，即初始进度</param>
			/// <param name="width">进度条的宽度，默认(-1)则自动匹配控制台宽度</param>
			ProgressBar(const std::string& title, size_t total_of_work, size_t initial_position = 0, int width = -1) {
				current = initial_position;
				total = total_of_work;
				if (width == -1) {
					this->width = Console().get_width();
				} else {
					this->width = width;
				}
				begin_time = clock_t();
				this->title = title + ' ';
			}

			// 重置进度条
			void reset() {
				current = 0;
				begin_time = clock();
			}
			// 增加进度
			ProgressBar& foward(int work_of_forward = 1) {
				if (current < total) {
					current = current + work_of_forward;
				} else {
					reset();
				}
				return *this;
			}
			// 设置进度
			void set_current(size_t current_work) {
				current = current_work % total;
			}
			// 设置总工作量
			void set_total(size_t total_of_work) {
				total = total_of_work;
			}
			// 设置宽度
			void set_width(size_t width) {
				this->width = width;
			}
			// 获取进度
			size_t get_current() const {
				return current;
			}
			// 获取总工作量
			size_t get_total() const {
				return total;
			}
			// 获取已用时间
			int get_time_spent() const {
				return float(clock() - begin_time) / CLOCKS_PER_SEC;
			}
			// 获取估计剩余时间 (秒)
			int get_eta() const {
				float sec = float(clock() - begin_time) / CLOCKS_PER_SEC;
				return sec / current * (total - current);
			}
			// 获取每秒完成的工作量
			float get_itps() const {
				return current / get_time_spent();
			}
			// 工作量的单位是字节
			void work_of_bytes(bool is_work_of_bytes) {
				is_bytes = is_work_of_bytes;
			}
			// 打印进度条
			ProgressBar& print(bool clear_line = true) {
				if (clear_line) {
					fputs("\033[2K\r", stdout);
				}
				fputs(to_string().c_str(), stdout);
				return *this;
			}

			// 核心代码

			// 获取进度条字符串
			std::string to_string() {
				/*
				* Example
				* 100%|█████████| 128/128 [00:02<00:00, 58.55it/s]
				*/
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
					if (current == 0 || sec == 0) {
						sprintf_s(suf, "| %s/%s [%02ld:%02ld<?, ?B/s]",
							funciton::byte_to_string(current).c_str(),
							funciton::byte_to_string(total).c_str(),
							int(sec) / 60, int(sec) % 60);
					} else { // _B/s
						sprintf_s(suf, "| %s/%s [%02d:%02ld<%02d:%02ld, %s/s]",
							funciton::byte_to_string(current).c_str(),
							funciton::byte_to_string(total).c_str(),
							int(sec) / 60, int(sec) % 60, eta / 60, eta % 60,
							funciton::byte_to_string(current / sec).c_str());
					}
				} else {
					if (current == 0 || sec == 0) {
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
					str += block_strs[8];
				}
				str += block_strs[int((blocks - int(blocks)) * 8)];
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