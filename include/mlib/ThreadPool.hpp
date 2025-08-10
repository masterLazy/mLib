#pragma once
/**
 * @file		ThreadPool.hpp
 * @brief		�򵥵��̳߳�
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include <thread>
#include <atomic>
#include <vector>
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
		/** @brief	���������� */
		typedef unsigned long long taskId_t;

		/**
		* @brief	�򵥵��̳߳�
		*/
		template<typename TaskType> class ThreadPool {
		public:
			typedef std::function<void(TaskType, const Signal&)> workFunc_t;

		private:
			struct TaskEx {
				TaskType task;
				taskId_t id;
			};

		public:
			/**
			* @brief			��ʼ���̳߳�
			* @param count		�߳�����
			* @param workFunc	ִ��ҵ��ĺ���
			*/
			ThreadPool(size_t count, const workFunc_t& workFunc) : count(count), workFunc(workFunc) {
				countAlive.store(0);
				for (size_t i = 0; i < count; i++) {
					threads.push_back(new std::thread(&ThreadPool::threadHandler, this));
					threads[i]->detach();
				}
			}
			~ThreadPool() {
				if (not terminated) {
					allExit();
					std::unique_lock<std::mutex> lk(mtx);
					cvExit.wait(lk, [this] { return countAlive == 0; });
				}
				for (size_t i = 0; i < count; i++) {
					delete threads[i];
				}
			}

			/** 
			 * @brief	������񵽶��� 
			 * @return	��һ�޶���������
			 */
			taskId_t addTask(const TaskType& new_task) {
				std::unique_lock<std::mutex> lk(mtx);
				tasks.push_back({ new_task, taskId});
				cv.notify_one(); // ֻ��һ������
				return taskId++;
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

			/** @brief	��ȡ��ǰ�����߳����� */
			size_t getAliveThreads() const {
				return countAlive;
			}

			/** @brief	��ȡ������г��� */
			size_t getPendingTasks() const {
				return tasks.size();
			}

			/** 
			 * @brief		�Ƴ�ָ������ 
			 * @param id	Ҫ�Ƴ���������
			 * @return		���񲻴���ʱ���� false
			 */
			bool removeTask(taskId_t id) {
				std::unique_lock<std::mutex> lk(mtx);
				for (auto i = tasks.begin(); i != tasks.end(); i++) {
					if (i->id == id) {
						tasks.erase(i);
						return true;
					}
				}
				return false;
			}

			/** @brief	����������� */
			void clearTasks() {
				std::unique_lock<std::mutex> lk(mtx);
				tasks.clear();
			}

		private:
			size_t count;
			workFunc_t workFunc;
			taskId_t taskId = 0;
			std::atomic<size_t> countAlive;
			std::vector<std::thread*> threads;
			std::vector<TaskEx> tasks;
			Signal signal = pause;
			bool terminated = false;

			std::mutex mtx;
			std::condition_variable cv, cvExit;

			/** @brief	�߳���ڵ� */
			void threadHandler() {
				countAlive++;
				TaskEx task_ex;
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
							task_ex = tasks.front();
							tasks.erase(tasks.begin());
							mtx.unlock();
							workFunc(task_ex.task, signal);
						}
					} else if (signal == exit) {
						break;
					}
				}
				countAlive--;
				if (countAlive == 0) {
					cvExit.notify_all(); // �ֵ�����Կ�ʼ delete ��
				}
			}
		};
	} // namespace ThreadPool
} // namespace mlib