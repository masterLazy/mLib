#pragma once
/*****************************************************************************
* lin_algebra
* 线性代数
*
* Vector、Matrix：除了数乘及以后的操作，arr为空导致越界的问题
* 尚未解决
*****************************************************************************/

namespace mlib
{
	/*****************************************************************************
	* Vector
	* 向量
	*
	*****************************************************************************/
	class Vector
	{
	private:
		std::vector<Math_F> arr;
	public:
		Math_F over_range = 0;//越界专用返回

		Vector() _RST {}
		explicit Vector(const size_t rank) _RST
		{
			arr.resize(rank);
		}
		Vector(const std::vector<Math_F> _arr) _RST
		{
			arr = _arr;
		}

		//获取维数
		size_t size() const _RST
		{
			return arr.size();
		}

		//清零
		void clear() _RST
		{
			arr.clear();
		}

		//改大小（会清零）
		void resize(const size_t rank) _RST
		{
			clear();
			arr.resize(rank);
		}

		//访问
		Math_F get(const size_t i) const _RST
		{
			//越界了
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

		//全部填充为
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


		//加法（无定义则返回零向量）
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

		//减法（无定义则返回零向量）
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

		//乘法（无定义则返回零向量）
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

		//除法（无定义则返回零向量）
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

		//数乘
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

		//数除
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


		//求和
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


		//限制最大值
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
		//限制最小值
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

		//将所有元素做指定运算
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
	* 矩阵
	*
	*****************************************************************************/
	class Matrix
	{
	private:
		std::vector<Math_F> arr;
		size_t row = 0, col = 0;
	public:
		Math_F over_range = 0;//越界专用返回

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

		//获取行、列数
		size_t get_row() _RST
		{
			return row;
		}
		size_t get_col() _RST
		{
			return col;
		}

		//清空
		void clear() _RST
		{
			arr.clear();
			row = col = 0;
		}

		//改大小（会清零）
		void resize(const size_t _row, const size_t _col) _RST
		{
			clear();
			row = _row;
			col = _col;
			arr.resize(row * col);
		}

		//访问
		Math_F get(const size_t _row, const size_t _col) const _RST
		{
			//越界了
			if (_row >= row || _col >= col)
			{
				return over_range;
			}
			return arr[_row * col + _col];
		}
		Math_F& operator()(const size_t _row, const size_t _col) _RST
		{
			//越界了
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

		//全部填充为
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


		//加法（无定义则返回零矩阵）
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

		//减法（无定义则返回零矩阵）
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

		//矩阵乘法（无定义则返回零矩阵）
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


		//数乘
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

		//数除
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

		//向量乘（col==v.size()，返回.size()==row）（无定义则返回零向量）
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


		//求和
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

		//展开成向量
		Vector to_vector() const _RST
		{
			return Vector(arr);
		}


		//限制最大值
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
		//限制最小值
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

		//将所有元素做指定运算
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