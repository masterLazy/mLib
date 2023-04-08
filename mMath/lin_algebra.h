#pragma once
/*****************************************************************************
* lin_algebra
* ���Դ���
*
* Vector��Matrix���������˼��Ժ�Ĳ�����arrΪ�յ���Խ�������
* ��δ���
*****************************************************************************/

namespace mlib
{
	/*****************************************************************************
	* Vector
	* ����
	*
	*****************************************************************************/
	class Vector
	{
	private:
		std::vector<Math_F> arr;
	public:
		Math_F over_range = 0;//Խ��ר�÷���

		Vector() _RST {}
		explicit Vector(const size_t rank) _RST
		{
			arr.resize(rank);
		}
		Vector(const std::vector<Math_F> _arr) _RST
		{
			arr = _arr;
		}

		//��ȡά��
		size_t size() const _RST
		{
			return arr.size();
		}

		//����
		void clear() _RST
		{
			arr.clear();
		}

		//�Ĵ�С�������㣩
		void resize(const size_t rank) _RST
		{
			clear();
			arr.resize(rank);
		}

		//����
		Math_F get(const size_t i) const _RST
		{
			//Խ����
			if (i >= arr.size())return over_range;
			return arr[i];
		}
		Math_F& operator[](const size_t i) _RST
		{
			if (i >= arr.size())return over_range;
			return arr[i];
		}
		std::vector<Math_F> get_arr_c() const _RST
		{
			return arr;
		}
		std::vector<Math_F>* get_arr() _RST
		{
			return &arr;
		}

		//ȫ�����Ϊ
		void fill(Math_F a) _RST
		{
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] = a;
				}
			}
		}


		//�ӷ����޶����򷵻���������
		Vector operator+(const Vector& v) const _RST
		{
			Vector res = *this;
			if (v.arr.size() == arr.size())
			{
				for (int i = 0; i < arr.size(); i++)
				{
					res[i] += v.arr[i];
				}
			}
			return res;
		}
		Vector operator+=(const Vector& v) _RST
		{
			if (v.arr.size() == arr.size())
			{
				for (int i = 0; i < arr.size(); i++)
				{
					arr[i] += v.arr[i];
				}
			}
			return *this;
		}

		//�������޶����򷵻���������
		Vector operator-(const Vector& v) const _RST
		{
			Vector res = *this;
			if (v.arr.size() == arr.size())
			{
				for (int i = 0; i < arr.size(); i++)
				{
					res[i] -= v.arr[i];
				}
			}
			return res;
		}
		Vector operator-=(const Vector& v) _RST
		{
			if (v.arr.size() == arr.size())
			{
				for (int i = 0; i < arr.size(); i++)
				{
					arr[i] -= v.arr[i];
				}
			}
			return *this;
		}

		//�˷����޶����򷵻���������
		Vector operator*(const Vector& v) const _RST
		{
			Vector res = *this;
			if (v.arr.size() == arr.size())
			{
				for (int i = 0; i < arr.size(); i++)
				{
					res[i] *= v.arr[i];
				}
			}
			return res;
		}
		Vector operator*=(const Vector& v) _RST
		{
			if (v.arr.size() == arr.size())
			{
				for (int i = 0; i < arr.size(); i++)
				{
					arr[i] *= v.arr[i];
				}
			}
			return *this;
		}

		//�������޶����򷵻���������
		Vector operator/(const Vector& v) const _RST
		{
			Vector res = *this;
			if (v.arr.size() == arr.size())
			{
				for (int i = 0; i < arr.size(); i++)
				{
					res[i] /= v.arr[i];
				}
			}
			return res;
		}
		Vector operator/=(const Vector& v) _RST
		{
			if (v.arr.size() == arr.size())
			{
				for (int i = 0; i < arr.size(); i++)
				{
					arr[i] /= v.arr[i];
				}
			}
			return *this;
		}

		//����
		Vector operator*(Math_F a) const _RST
		{
			Vector res = *this;
			if (!arr.empty())
			{
				for (int i = 0; i < arr.size(); i++)
				{
					res[i] *= a;
				}
			}
			return res;
		}
		Vector operator*=(Math_F a)  _RST
		{
			if (!arr.empty())
			{
				for (int i = 0; i < arr.size(); i++)
				{
					arr[i] *= a;
				}
			}
			return *this;
		}
		friend Vector operator*(Math_F a, const Vector& v) _RST
		{
			Vector res(v.size());
			if (!v.arr.empty())
			{
				for (int i = 0; i < v.size(); i++)
				{
					res[i] *= a;
				}
			}
			return res;
		}

		//����
		Vector operator/(Math_F a) const _RST
		{
			Vector res = *this;
			if (!arr.empty())
			{
				for (size_t i = 0; i < res.arr.size(); i++)
				{
					res.arr[i] /= a;
				}
			}
			return res;
		}
		Vector operator/=(Math_F a) _RST
		{
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] /= a;
				}
			}
			return *this;
		}
		Vector friend operator/(Math_F a, const Vector& v)
		{
			Vector res = v;
			if (!v.arr.empty())
			{
				for (size_t i = 0; i < res.arr.size(); i++)
				{
					res.arr[i] = a / res.arr[i];
				}
			}
			return res;
		}


		//���
		Math_F sum() const _RST
		{
			Math_F res = 0;
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					res += arr[i];
				}
			}
			return res;
		}


		//�������ֵ
		void max_limit(Math_F max_l)
		{
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] = max(arr[i], max_l);
				}
			}
		}
		//������Сֵ
		void min_limit(Math_F min_l)
		{
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] = min(arr[i], min_l);
				}
			}
		}

		//������Ԫ����ָ������
		template<typename T>
		void work(T(*fun)(T)) _RST
		{
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] = (*fun)(arr[i]);
				}
			}
		}
	};

	/*****************************************************************************
	* Matrix
	* ����
	*
	*****************************************************************************/
	class Matrix
	{
	private:
		std::vector<Math_F> arr;
		size_t row = 0, col = 0;
	public:
		Math_F over_range = 0;//Խ��ר�÷���

		Matrix() _RST {}
		Matrix(const size_t _row, const size_t _col) _RST
		{
			row = _row;
			col = _col;
			arr.resize(row * col);
		}
		Matrix(const size_t _row, const size_t _col, const std::vector<Math_F> _arr) _RST
		{
			row = _row;
			col = _col;

			arr = _arr;
			arr.resize(row * col);
		}
		Matrix(const size_t _row, const size_t _col, const Vector vec) _RST
		{
			row = _row;
			col = _col;

			arr = vec.get_arr_c();
			arr.resize(row * col);
		}

		//��ȡ�С�����
		size_t get_row() _RST
		{
			return row;
		}
		size_t get_col() _RST
		{
			return col;
		}

		//���
		void clear() _RST
		{
			arr.clear();
			row = col = 0;
		}

		//�Ĵ�С�������㣩
		void resize(const size_t _row, const size_t _col) _RST
		{
			clear();
			row = _row;
			col = _col;
			arr.resize(row * col);
		}

		//����
		Math_F get(const size_t _row, const size_t _col) const _RST
		{
			//Խ����
			if (_row >= row || _col >= col)
			{
				return over_range;
			}
			return arr[_row * col + _col];
		}
		Math_F& operator()(const size_t _row, const size_t _col) _RST
		{
			//Խ����
			if (_row >= row || _col >= col)
			{
				return over_range;
			}
			return arr[_row * col + _col];
		}
		std::vector<Math_F> get_arr_c() const _RST
		{
			return arr;
		}
		std::vector<Math_F>* get_arr() _RST
		{
			return &arr;
		}

		//ȫ�����Ϊ
		void fill(Math_F a) _RST
		{
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] = a;
				}
			}
		}


		//�ӷ����޶����򷵻������
		Matrix operator+(const Matrix& m) const _RST
		{
			Matrix res;
			if (m.row == row && m.col == col)
			{
				for (size_t i = 0; i < res.arr.size(); i++)
				{
					res.arr[i] += m.arr[i];
				}
			}
			return res;
		}
		Matrix operator+=(const Matrix& m) _RST
		{
			if (m.row == row && m.col == col)
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] += m.arr[i];
				}
			}
			return *this;
		}

		//�������޶����򷵻������
		Matrix operator-(const Matrix& m) const _RST
		{
			Matrix res;
			if (m.row == row && m.col == col)
			{
				for (size_t i = 0; i < res.arr.size(); i++)
				{
					res.arr[i] -= m.arr[i];
				}
			}
			return res;
		}
		Matrix operator-=(const Matrix& m) _RST
		{
			if (m.row == row && m.col == col)
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] -= m.arr[i];
				}
			}
			return *this;
		}

		//����˷����޶����򷵻������
		Matrix operator*(const Matrix& m)
		{
			Matrix res(row, m.col);
			if (m.row == col && m.col == row)
			{
				for (size_t i = 0; i < res.row; i++)
				{
					for (size_t j = 0; j < res.col; j++)
					{
						for (size_t k = 0; k < col; k++)
						{
							res(i, j) += get(i, k) * m.get(k, j);
						}
					}
				}
			}
			return res;
		}


		//����
		Matrix operator*(Math_F a) const _RST
		{
			Matrix res = *this;
			if (!arr.empty())
			{
				for (size_t i = 0; i < res.arr.size(); i++)
				{
					res.arr[i] *= a;
				}
			}
			return res;
		}
		Matrix operator*=(Math_F a) _RST
		{
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] *= a;
				}
			}
			return *this;
		}
		Matrix friend operator*(Math_F a, const Matrix& m)
		{
			Matrix res = m;
			if (!m.arr.empty())
			{
				for (size_t i = 0; i < res.arr.size(); i++)
				{
					res.arr[i] *= a;
				}
			}
			return res;
		}

		//����
		Matrix operator/(Math_F a) const _RST
		{
			Matrix res = *this;
			if (!arr.empty())
			{
				for (size_t i = 0; i < res.arr.size(); i++)
				{
					res.arr[i] /= a;
				}
			}
			return res;
		}
		Matrix operator/=(Math_F a) _RST
		{
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] /= a;
				}
			}
			return *this;
		}
		Matrix friend operator/(Math_F a, const Matrix& m)
		{
			Matrix res = m;
			if (!m.arr.empty())
			{
				for (size_t i = 0; i < res.arr.size(); i++)
				{
					res.arr[i] = a / res.arr[i];
				}
			}
			return res;
		}

		//�����ˣ�col==v.size()������.size()==row�����޶����򷵻���������
		Vector operator*(const Vector& v)
		{
			Vector res(row);
			if (col == v.size())
			{
				for (size_t i = 0; i < row; i++)
				{
					for (size_t j = 0; j < col; j++)
					{
						res[i] += get(i, j) * v.get(j);
					}
				}
			}
			return res;
		}


		//���
		Math_F sum() const _RST
		{
			Math_F res = 0;
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					res += arr[i];
				}
			}
			return res;
		}

		//չ��������
		Vector to_vector() const _RST
		{
			return Vector(arr);
		}


		//�������ֵ
		void max_limit(Math_F max_l)
		{
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] = max(arr[i], max_l);
				}
			}
		}
		//������Сֵ
		void min_limit(Math_F min_l)
		{
			if (!arr.empty())
			{
				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i] = min(arr[i], min_l);
				}
			}
		}

		//������Ԫ����ָ������
		template<typename T>
		void work(T(*fun)(T))
		{
			for (size_t i = 0; i < arr.size(); i++)
			{
				arr[i] = (*fun)(arr[i]);
			}
		}
	};
}