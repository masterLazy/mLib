#pragma once
/*
* [共享]
* 返回结果信息
*/
#include <string>

namespace mlib {
	class Result {
	public:
		bool isSuccess;
		std::wstring msg;

		static const Result success(const std::wstring& _msg = L"操作成功。") {
			Result res;
			res.isSuccess = true;
			res.msg = _msg;
			return res;
		}
		static void success(Result* res, const std::wstring& _msg = L"操作成功。") {
			if (res != nullptr) {
				*res = success(_msg);
			}
		}

		static const Result fail(const std::wstring& _msg = L"操作失败。") {
			Result res;
			res.isSuccess = false;
			res.msg = _msg;
			return res;
		}
		static void fail(Result* res, const std::wstring& _msg = L"操作失败。") {
			if (res != nullptr) {
				*res = fail(_msg);
			}
		}

		static const Result pass(const std::wstring& _msg = L"操作忽略。") {
			Result res;
			res.isSuccess = true;
			res.msg = _msg;
			return res;
		}
		static void pass(Result* res, const std::wstring& _msg = L"操作忽略。") {
			if (res != nullptr) {
				*res = pass(_msg);
			}
		}
	};
} // namespace mlib