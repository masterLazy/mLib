#pragma once
/*****************************************************************************
* numEx.h
* �߾�������
*
*****************************************************************************/

namespace mlib
{
	/*****************************************************************************
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
		IntEx(unsigned int x)
		{
			while (x > 0)
			{
				arr.insert(arr.begin(), x % 10);
				x /= 10;
			}
		}
		IntEx(int x)
		{
			if (x < 0)
			{
				neg = true;
				x = -x;
			}
			while (x > 0)
			{
				arr.insert(arr.begin(), x % 10);
				x /= 10;
			}
		}
		IntEx(const std::string& x)
		{
			//�Ϸ������
			for (size_t i = 0; i < x.size(); i++)
			{
				if (x[i] < '0' || x[i] > '9')
				{
					if (i == 0 && (x[i] == '+' || x[i] == '-'))
					{
						continue;
					}
					return;
				}
			}
			//�ж�����
			size_t l = 0;
			if (x[0] == '+')
			{
				neg = false;
				l = 1;
			}
			else if (x[0] == '-')
			{
				neg = true;
				l = 1;
			}
			//��ʼ
			bool begin = false;
			for (size_t i = l; i < x.size(); i++)
			{
				if (!begin && x[i] != '0')begin = true;
				if (begin)arr.push_back(x[i] - '0');
			}
		}

		//���ֳ���
		size_t size() const
		{
			return arr.size();
		}

		//תΪ string
		std::string get_str(bool positive_sign = false) const
		{
			std::string res;
			if (neg)
			{
				res += '-';
			}
			else if (positive_sign)
			{
				res += '+';
			}
			for (auto ch : arr)res += ch + '0';
			return res;
		}

		//�Ƚϴ�С
		bool operator==(const IntEx& b) const
		{
			//�ӷ���
			if (neg != b.neg)return false;
			//�ӳ���
			if (size() != b.size())return false;
			//������
			for (size_t i = 0; i < size(); i++)
			{
				if (arr[i] != b.arr[i])return false;
			}
			return true;
		}
		bool operator!=(const IntEx& b) const
		{
			return !(*this == b);
		}
		bool operator>(const IntEx& b) const
		{
			//�ӷ���
			if (!neg && b.neg)return true;
			else if (neg && !b.neg)return false;
			//�ӳ���
			if (size() > b.size())
			{
				if (!neg)return true;
				else return false;
			}
			if (size() < b.size())
			{
				if (!neg)return false;
				else return true;
			}
			//������
			for (size_t i = 0; i < size(); i++)
			{
				if (arr[i] > b.arr[i])
				{
					if (!neg)return true;
					else return false;
				}
				if (arr[i] < b.arr[i])
				{
					if (!neg)return false;
					else return true;
				}
			}
			return false;
		}
		bool operator>=(const IntEx& b) const
		{
			return *this > b || *this == b;
		}
		bool operator<(const IntEx& b) const
		{
			return *this <= b && *this != b;
		}
		bool operator<=(const IntEx& b) const
		{
			return !(*this > b);
		}

		//����
		IntEx operator-() const
		{
			IntEx res = *this;
			res.neg = !neg;
			return res;
		}

		IntEx operator+(const IntEx& b) const
		{
			//ͬ��
			if (neg == b.neg)
			{
				IntEx res;
				size_t max_size = size() > b.size() ? size() : b.size();
				res.arr.resize(max_size + 1);
				res.neg = neg;
				//��ʼ����
				for (size_t i = 1; i <= max_size; i++)
				{
					if (i <= size())
					{
						res.arr[max_size + 1 - i] += arr[size() - i];
					}
					if (i <= b.size())
					{
						res.arr[max_size + 1 - i] += b.arr[b.size() - i];
					}
					//��λ
					if (res.arr[max_size + 1 - i] >= 10)
					{
						res.arr[max_size - i] += 1;
						res.arr[max_size + 1 - i] -= 10;
					}
				}
				//ȥ0
				if (res.arr[0] == 0 && res.size() > 1)res.arr.erase(res.arr.begin());
				return res;
			}
			//���
			else
			{
				if (neg)return b - (-*this);
				else return *this - (-b);
			}
		}
		IntEx operator-(const IntEx& b) const
		{
			//ͬ��
			if (neg == b.neg)
			{
				//this < b
				if (*this < b)
				{
					IntEx res = b - *this;
					res.neg = true;
					return res;
				}
				//this >= b
				else
				{
					IntEx res, a = *this;
					size_t max_size = size() > b.size() ? size() : b.size();
					res.arr.resize(max_size);
					res.neg = neg;
					//��ʼ����
					for (size_t i = 1; i <= max_size; i++)
					{
						//��λ
						if (a.arr[a.size() - i] < 0)
						{
							a.arr[a.size() - i] += 10;
							a.arr[a.size() - 1 - i] -= 1;
						}
						if (i <= a.size())
						{
							res.arr[max_size - i] += a.arr[a.size() - i];
						}
						if (i <= b.size())
						{
							//��λ
							if (a.arr[a.size() - i] < b.arr[b.size() - i])
							{
								res.arr[max_size - i] += 10;
								a.arr[a.size() - 1 - i] -= 1;
							}

							res.arr[max_size - i] -= b.arr[b.size() - i];
						}
					}
					//ȥ0
					while (res.arr[0] == 0 && res.size() > 1)res.arr.erase(res.arr.begin());
					return res;
				}
			}
			//���
			else
			{
				if (neg)return -((-*this) + b);
				else return *this + b;
			}
		}

		//����̨I/O
#ifdef _IOSTREAM_
		friend std::istream& operator>>(std::istream& is, IntEx& i)
		{
			std::string str;
			is >> str;
			i = IntEx(str);
			return is;
		}
		friend std::ostream& operator<<(std::ostream& os, const IntEx& i)
		{
			os << i.get_str();
			return os;
		}
#endif
	};
}