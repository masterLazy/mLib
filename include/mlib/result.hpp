#pragma once
/*
* [����]
* ���ؽ����Ϣ
*/
#include <string>

namespace mlib {
	class Result {
	public:
		bool isSuccess;
		std::wstring msg;

		static const Result success(const std::wstring& _msg = L"�����ɹ���") {
			Result res;
			res.isSuccess = true;
			res.msg = _msg;
			return res;
		}
		static void success(Result* res, const std::wstring& _msg = L"�����ɹ���") {
			if (res != nullptr) {
				*res = success(_msg);
			}
		}

		static const Result fail(const std::wstring& _msg = L"����ʧ�ܡ�") {
			Result res;
			res.isSuccess = false;
			res.msg = _msg;
			return res;
		}
		static void fail(Result* res, const std::wstring& _msg = L"����ʧ�ܡ�") {
			if (res != nullptr) {
				*res = fail(_msg);
			}
		}

		static const Result pass(const std::wstring& _msg = L"�������ԡ�") {
			Result res;
			res.isSuccess = true;
			res.msg = _msg;
			return res;
		}
		static void pass(Result* res, const std::wstring& _msg = L"�������ԡ�") {
			if (res != nullptr) {
				*res = pass(_msg);
			}
		}
	};
} // namespace mlib