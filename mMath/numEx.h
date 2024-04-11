#pragma once
/*****************************************************************************
* numEx.h
* �߾�������
*
*****************************************************************************/

namespace mlib
{
	/*****************************************************************************
	* IntEx
	* �߾�������
	*****************************************************************************/
	class IntEx
	{
	private:
		std::vector<char> arr;
		bool neg = false; //�Ƿ�Ϊ��
	public:
		//����
		IntEx() {}
		IntEx(unsigned int x);
		IntEx(int x);
		IntEx(const std::string& x);

		//���ֳ���
		size_t size() const;

		//תΪ string
		std::string get_str(bool positive_sign = false) const;

		//�Ƚϴ�С
		bool operator==(const IntEx& b) const;
		bool operator!=(const IntEx& b) const;
		bool operator>(const IntEx& b) const;
		bool operator>=(const IntEx& b) const;
		bool operator<(const IntEx& b) const;
		bool operator<=(const IntEx& b) const;

		//����
		IntEx operator-() const;

		IntEx operator+(const IntEx& b) const;
		IntEx operator-(const IntEx& b) const;

		//����̨I/O
#ifdef _IOSTREAM_
		friend std::istream& operator>>(std::istream& is, IntEx& i);
		friend std::ostream& operator<<(std::ostream& os, const IntEx& i);
#endif
	};
}