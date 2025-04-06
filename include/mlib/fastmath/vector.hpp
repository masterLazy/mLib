#pragma once
/*
* [����ӿ�]
* ���м�������
*/
#include "mlib/fastmath.hpp"
namespace mlib {
	namespace fastmath {
		template <typename Float, typename Derived> class Vector {
		public:
			// ��ȡ������С
			virtual size_t size() const = 0;
			// ��Ԫ��: ȡ�෴��
			virtual Derived& nagate(Result* res = nullptr) = 0;
			// ��Ԫ��: ���
			virtual Derived& add(const Derived& another, Result* res = nullptr) = 0;
			virtual Derived& add(Float f, Result* res = nullptr) = 0;
			// ��Ԫ��: ���
			virtual Derived& sub(const Derived& another, Result* res = nullptr) = 0;
			virtual Derived& sub(Float f, Result* res = nullptr) = 0;
			// ��Ԫ��: ���
			virtual Derived& mul(const Derived& another, Result* res = nullptr) = 0;
			virtual Derived& mul(Float f, Result* res = nullptr) = 0;
			// ��Ԫ��: ���
			virtual Derived& div(const Derived& another, Result* res = nullptr) = 0;
			virtual Derived& div(Float f, Result* res = nullptr) = 0;
			// ��Ԫ��: ����
			virtual Derived& pow(const Derived& another, Result* res = nullptr) = 0;
			virtual Derived& pow(Float f, Result* res = nullptr) = 0;
		};
	} // namespace fastmath
} // namespace mlib