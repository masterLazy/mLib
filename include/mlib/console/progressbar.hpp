#pragma once
/**
 * @file		progressbar.hpp
 * @brief		������
 * @details		��tdqm��ʽ
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include "mlib/console.hpp"
#include "mlib/function.hpp"

namespace mlib {
	namespace console {
		const std::string __block_strs[9] = {
			"",			// ��
			"\u258f",	// �� 1/8
			"\u258e",	// �� 2/8
			"\u258d",	// �� 3/8
			"\u258c",	// �� 4/8
			"\u258b",	// �� 5/8
			"\u258a",	// �� 6/8
			"\u2589",	// �� 7/8
			"\u2588"	// ��������
		};
		/**
		 * @brief		��eta���㹦�ܵĽ�����
		 */
		class ProgressBar {
			size_t current, total;
			clock_t begin_time;
			int width;
			std::string title;		// ĩβ���������һ���ո�
			bool is_bytes = false;	// �������ĵ�λ���ֽ�
		public:
			/**
			 * @brief					����һ��������
			 * @param total_of_work		�ܹ�����
			 * @param initial_position	��ʼλ��, ����ʼ����
			 * @param width				�������Ŀ��, Ĭ��(-1)���Զ�ƥ�����̨���
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
			 * @brief					����һ��������
			 * @param title				����������
			 * @param total_of_work		�ܹ�����
			 * @param initial_position	��ʼλ��, ����ʼ����
			 * @param width				�������Ŀ��, Ĭ��(-1)���Զ�ƥ�����̨���
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

			/** @brief	���ý����� */
			void reset() {
				current = 0;
				begin_time = clock();
			}
			/**
			 * @brief					���ӽ���
			 * @param work_of_forward	Ҫ���ӵĹ�����
			 * @return					���ĺ�Ľ���������
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
			 * @brief					���ý���
			 * @param current_work		��ǰ������
			 */
			void setCurrent(size_t current_work) {
				current = current_work % total;
			}
			/**
			 * @brief					�����ܹ�����
			 * @param total_of_work		�ܹ�����
			 */
			void setTotal(size_t total_of_work) {
				total = total_of_work;
			}
			/**
			 * @brief			���ÿ��
			 * @param width		���
			 */
			void setWidth(size_t width) {
				this->width = width;
			}
			/** @brief		��ȡ��ǰ��ɵĹ�����  */
			size_t getCurrent() const {
				return current;
			}
			/**  @brief		��ȡ�ܹ����� */
			size_t getTotal() const {
				return total;
			}
			/**
			 * @brief		��ȡ����ʱ��
			 * @return		����ʱ��(s)
			 */
			int getTimeSpent() const {
				return float(clock() - begin_time) / CLOCKS_PER_SEC;
			}
			/**  @brief		��ȡ����ʣ��ʱ��(s) */
			int getEta() const {
				float sec = float(clock() - begin_time) / CLOCKS_PER_SEC;
				return sec / current * (total - current);
			}
			/** @brief		��ȡÿ����ɵĹ����� */
			float getItps() const {
				return current / getTimeSpent();
			}
			/**
			 * @brief		��֪�������ĵ�λ���ֽ�
			 * @details		�����������ֽ�����ʽ��ʾ
			 */
			void workOfBytes(bool is_work_of_bytes) {
				is_bytes = is_work_of_bytes;
			}
			/**
			 * @brief		��cout��ӡ������
			 * @return		����������
			 */
			ProgressBar& print(bool clearLine = true) {
				if (clearLine) {
					// console::clearLine();
					std::cout << "\r";
				}
				std::cout << toString();
				return *this;
			}

			// ���Ĵ���

			/**
			 * @brief		��ȡ�������ַ���
			 * @example		100%|������������������| 128/128 [00:02<00:00, 58.55it/s]
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
				// ��������������
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