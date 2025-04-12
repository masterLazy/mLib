#pragma once
/**
 * @file		result.hpp
 * @brief		操作结果状态包装器
 *
 * @author		masterLazy
 * @copyright	Copyright (c) 2025 masterLazy
 */

#include <string>

namespace mlib {
	namespace result {
		/**
		 * @brief	操作结果状态包装器
		 * @details	包含状态(是否成功)和一条简短说明
		 */
		class Result {
		public:
			enum class State {
				success,	// 操作成功
				fail,		// 操作失败
				pass		// 操作忽略
			};
		private:
			State state;
			std::string msg;
		public:
			/**
			 * @param state		操作状态
			 * @param msg		状态信息
			 */
			Result(State state, const std::string& msg) : state(state), msg(msg) {}

			/**
			 * @brief 	包装一个成功状态
			 * @return	包装结果
			 */
			static const Result sucess(std::string msg = "") {
				return Result(State::success, msg);
			}

			/**
			 * @brief 	包装一个失败状态
			 * @return	包装结果
			 */
			static const Result fail(std::string msg = "") {
				return Result(State::fail, msg);
			}

			/**
			 * @brief 	包装一个忽略状态
			 * @return	包装结果
			 */
			static const Result pass(std::string msg = "") {
				return Result(State::pass, msg);
			}

			/**
			 * @brief			操作是否成功
			 * @retval true		操作成功或忽略
			 * @retval false	操作失败
			 */
			bool isSuccess() const {
				return state == State::success or state == State::pass;
			}

			/**
			 * @brief	操作是否被忽略
			 * @return	判断结果
			 */
			bool isPass() const {
				return state == State::pass;
			}

			/**
			 * @brief	获取状态信息
			 * @return	状态信息
			 */
			std::string what() const {
				return msg;
			}
		};

		/**
		 * @brief		包装一个成功状态，存到指定指针中
		 * @param res	存储状态的指针
		 */
		void handlerSuccess(Result* res, std::string msg = "") {
			if (res != nullptr) *res = Result::sucess(msg);
		}

		/**
		 * @brief		包装一个失败状态，存到指定指针中
		 * @param res	存储状态的指针
		 */
		void handlerFail(Result* res, std::string msg = "") {
			if (res != nullptr) *res = Result::fail(msg);
		}

		/**
		 * @brief		包装一个忽略状态，存到指定指针中
		 * @param res	存储状态的指针
		 */
		void handlerPass(Result* res, std::string msg = "") {
			if (res != nullptr) *res = Result::pass(msg);
		}
	} // namespace result
} // namespace mlib