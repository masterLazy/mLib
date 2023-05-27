#pragma once
/*****************************************************************************
* num
* ��ά����(��NumPy)
*
*****************************************************************************/

#define NUM_H

namespace mlib
{
	namespace np
	{
		//����
		template <typename T>
		class array
		{
		private:
			T* arr = nullptr;
			size_t size = 0;
			bool not_delete = false;

			void clear()
			{
				if (arr != nullptr && !not_delete)
				{
					delete arr;
				}
				size = 0;
			}

		public:
			std::vector<size_t> shape = { 0 };

			array() {}
			//�൱�� zeros / ones / empty
			 explicit array(std::vector<size_t> _shape, T value = 0)
			{
				shape = _shape;

				size_t _size = 1;
				for (size_t i = 0; i < _shape.size(); i++)
				{
					_size *= _shape[i];
				}
				size = _size;

				arr = new T[size];
				for (int i = 0; i < size; i++)arr[i] = value;
			}
			~array() { clear(); }

			//ת��
			template<typename T1>
			array(const std::vector<T1>& _arr)
			{
				clear();
				shape = { _arr.size() };
				size = _arr.size();
				arr = new T[size];
				for (size_t i = 0; i < size; i++)
				{
					arr[i] = _arr[i];
				}
			}
			template<typename T1>
			array(const T1 _arr[], size_t _size)
			{
				clear();
				shape = { _size };
				size = _size;
				arr = new T[size];
				for (size_t i = 0; i < size; i++)
				{
					arr[i] = _arr[i];
				}
			}

			//����shape
			bool reshape(const std::vector<size_t> _shape)
			{
				//���һ��
				size_t _size = 1;
				for (size_t i = 0; i < _shape.size(); i++)
				{
					_size *= _shape[i];
				}
				if (_size != size)
				{
#ifdef _IOSTREAM_
#ifndef NDEBUG
					std::cerr << "mlib::mMath::num: reshape() failed.(non-fatal)" << std::endl;
#endif
#endif
					return false;
				}
				shape = _shape;
				return true;
			}

			//����(���Ҫת��!)
			array operator[](size_t n) const
			{
				if (!(n < shape[0]))
				{
					//Խ��
#ifdef _IOSTREAM_
#ifndef NDEBUG
					std::cerr << "mlib::mMath::num: Array subscript out of range." << std::endl;
#endif
#endif
					exit(-1);
				}

				array res;
				res.arr = arr + size / shape[0] * n;
				res.size = size / shape[0];
				res.shape = shape;
				res.shape.erase(res.shape.begin());
				if (res.shape.empty())res.shape = { 1 };
				res.not_delete = true;
				return res;
			}
			//ת����T
			operator T&()
			{
				if (!(size == 1))
				{
					//�޷�ת��
#ifdef _IOSTREAM_
#ifndef NDEBUG
					std::cerr << "mlib::mMath::num: Cannot change an array into a value." << std::endl;
#endif
#endif
					exit(-1);
				}

				return arr[0];
			}
			//�������ת��
			T& operator()()
			{
				return (T&)*this;
			}

			//���
#ifdef _IOSTREAM_
			friend std::ostream& operator<<(std::ostream& os, const array& arr)
			{
				//һά
				if (arr.shape.size() == 1)
				{
					bool first = true;
					os << "[";
					for (size_t i = 0; i < arr.shape[0]; i++)
					{
						if (first)first = false;
						else os << " ";
						os << (T)arr[i];
					}
					os << "]";
				}
				//��ά
				else if (arr.shape.size() == 2)
				{
					for (size_t y = 0; y < arr.shape[0]; y++)
					{
						if (y == 0)os << "[";
						else os << " ";
						bool first = true;
						os << "[";
						for (size_t x = 0; x < arr.shape[1]; x++)
						{
							if (first)first = false;
							else os << " ";
							os << (T)arr[y][x];
						}
						os << "]";
						if (y + 1 != arr.shape[0])os << std::endl;
					}
					os << "]";
				}
				return os;
			}
#endif
		};
	}
}