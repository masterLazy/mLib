#pragma once
/**
 * @file		ThreadPool.hpp
 * @brief		�������̳߳�
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include <thread>
#include <atomic>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <Windows.h>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace mlib {
	namespace ThreadPool {
		/** @brief	�߳��ź� */
		enum Signal { pause, run, exit };

		/**
		* @brief	�̳߳�
		* @details	TaskType
		*/
		template<typename TaskType> class ThreadPool {
		public:
			typedef std::function<void(TaskType, const Signal&)> workFunc_t;

			/**
			* @brief			��ʼ���̳߳�
			* @param count		�߳�����
			* @param work_func	ִ��ҵ��ĺ���
			*/
			ThreadPool(size_t count, const workFunc_t& work_func) : count(count), work_func(work_func) {
				count_active.store(0);
				for (size_t i = 0; i < count; i++) {
					threads.push_back(new std::thread(&ThreadPool::threadHandler, this));
					threads[i]->detach();
				}
			}
			~ThreadPool() {
				if (not terminated) {
					allExit();
					std::unique_lock<std::mutex> lk(mtx);
					exit_cv.wait(lk, [this] { return count_active == 0; });
				}
				for (size_t i = 0; i < count; i++) {
					delete threads[i];
				}
			}

			/** @brief ������񵽶��� */
			void pushTask(const TaskType& new_task) {
				std::unique_lock<std::mutex> lk(mtx);
				tasks.push(new_task);
				cv.notify_one(); // ֻ��һ������
			}

			/** @brief	֪ͨ�����߳̿�ʼ���� */
			void allRun() {
				std::unique_lock<std::mutex> lk(mtx);
				signal = run;
				cv.notify_all();
			}
			/** @brief	֪ͨ�����߳���ͣ���� */
			void allPause() {
				std::unique_lock<std::mutex> lk(mtx);
				signal = pause;
				cv.notify_all();
			}
			/** @brief	֪ͨ�����߳��˳� */
			void allExit() {
				std::unique_lock<std::mutex> lk(mtx);
				signal = exit;
				cv.notify_all();
			}
			/** @brief ǿ����ֹ�����߳� (Σ��) */
			bool allTerminate() {
				if (count = 0) return false;
				bool res = true;
				for (auto i : threads) {
					res &= TerminateThread(i->native_handle(), -1);
				}
				terminated = true;
				return res;
			}

			/** @brief	��ȡ��ǰ��Ծ���߳����� */
			size_t getActiveThreads() const {
				return count_active;
			}

			/** @brief	��ȡ������г��� */
			size_t getPendingTasks() const {
				return tasks.size();
			}

			/** @brief	����������� */
			void clearTasks() {
				std::unique_lock<std::mutex> lk(mtx);
				tasks = std::queue<TaskType>();
			}

		//private:
			size_t count;
			workFunc_t work_func;
			std::atomic<size_t> count_active;
			std::vector<std::thread*> threads;
			std::queue<TaskType> tasks;
			Signal signal = pause;
			bool terminated = false;

			std::mutex mtx;
			std::condition_variable cv, exit_cv;

			/** @brief	�߳���ڵ� */
			void threadHandler() {
				count_active++;
				TaskType task;
				while (true) {
					{
						std::unique_lock<std::mutex> lk(mtx);
						cv.wait(lk, [this]() {
							return not (signal == run and tasks.empty());
							});
					}
					if (signal == run) {
						mtx.lock();
						if (signal == pause or tasks.empty()) { // ������
							mtx.unlock();
						} else {
							task = tasks.front();
							tasks.pop();
							mtx.unlock();
							work_func(task, signal);
						}
					} else if (signal == exit) {
						break;
					}
				}
				count_active--;
				if (count_active == 0) {
					exit_cv.notify_all(); // �ֵ�����Կ�ʼ delete ��
				}
			}
		};
	} // namespace ThreadPool
} // namespace mlib