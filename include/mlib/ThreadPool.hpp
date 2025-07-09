#pragma once
/**
 * @file		ThreadPool.hpp
 * @brief		高性能线程池
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
		/** @brief	线程信号 */
		enum Signal { pause, run, exit };

		/**
		* @brief	线程池
		* @details	TaskType
		*/
		template<typename TaskType> class ThreadPool {
		public:
			typedef std::function<void(TaskType, const Signal&)> workFunc_t;

			/**
			* @brief			初始化线程池
			* @param count		线程数量
			* @param work_func	执行业务的函数
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

			/** @brief 添加任务到队列 */
			void pushTask(const TaskType& new_task) {
				std::unique_lock<std::mutex> lk(mtx);
				tasks.push(new_task);
				cv.notify_one(); // 只让一个来抢
			}

			/** @brief	通知所有线程开始工作 */
			void allRun() {
				std::unique_lock<std::mutex> lk(mtx);
				signal = run;
				cv.notify_all();
			}
			/** @brief	通知所有线程暂停工作 */
			void allPause() {
				std::unique_lock<std::mutex> lk(mtx);
				signal = pause;
				cv.notify_all();
			}
			/** @brief	通知所有线程退出 */
			void allExit() {
				std::unique_lock<std::mutex> lk(mtx);
				signal = exit;
				cv.notify_all();
			}
			/** @brief 强制终止所有线程 (危险) */
			bool allTerminate() {
				if (count = 0) return false;
				bool res = true;
				for (auto i : threads) {
					res &= TerminateThread(i->native_handle(), -1);
				}
				terminated = true;
				return res;
			}

			/** @brief	获取当前活跃的线程数量 */
			size_t getActiveThreads() const {
				return count_active;
			}

			/** @brief	获取任务队列长度 */
			size_t getPendingTasks() const {
				return tasks.size();
			}

			/** @brief	清空所有任务 */
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

			/** @brief	线程入口点 */
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
						if (signal == pause or tasks.empty()) { // 不干了
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
					exit_cv.notify_all(); // 兄弟你可以开始 delete 了
				}
			}
		};
	} // namespace ThreadPool
} // namespace mlib